/*
 * driver/audio/audio.c
 *
 * Copyright(c) 2007-2023 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <xboot.h>
#include <audio/audio.h>

static ssize_t audio_read_playback_volume(struct kobj_t * kobj, void * buf, size_t size)
{
	struct audio_t * audio = (struct audio_t *)kobj->priv;
	return sprintf(buf, "%d", audio_get_playback_volume(audio));
}

static ssize_t audio_write_playback_volume(struct kobj_t * kobj, void * buf, size_t size)
{
	struct audio_t * audio = (struct audio_t *)kobj->priv;
	audio_set_playback_volume(audio, strtol(buf, NULL, 0));
	return size;
}

static ssize_t audio_read_capture_volume(struct kobj_t * kobj, void * buf, size_t size)
{
	struct audio_t * audio = (struct audio_t *)kobj->priv;
	return sprintf(buf, "%d", audio_get_capture_volume(audio));
}

static ssize_t audio_write_capture_volume(struct kobj_t * kobj, void * buf, size_t size)
{
	struct audio_t * audio = (struct audio_t *)kobj->priv;
	audio_set_capture_volume(audio, strtol(buf, NULL, 0));
	return size;
}

struct audio_t * search_audio(const char * name)
{
	struct device_t * dev;

	dev = search_device(name, DEVICE_TYPE_AUDIO);
	if(!dev)
		return NULL;
	return (struct audio_t *)dev->priv;
}

struct audio_t * search_first_audio(void)
{
	struct device_t * dev;

	dev = search_first_device(DEVICE_TYPE_AUDIO);
	if(!dev)
		return NULL;
	return (struct audio_t *)dev->priv;
}

struct device_t * register_audio(struct audio_t * audio, struct driver_t * drv)
{
	struct device_t * dev;

	if(!audio || !audio->name || (!audio_playback_is_support(audio) && !audio_capture_is_support(audio)))
		return NULL;

	dev = malloc(sizeof(struct device_t));
	if(!dev)
		return NULL;

	audio->soundpool.playback = NULL;
	init_list_head(&audio->soundpool.list);
	spin_lock_init(&audio->soundpool.lock);

	dev->name = strdup(audio->name);
	dev->type = DEVICE_TYPE_AUDIO;
	dev->driver = drv;
	dev->priv = audio;
	dev->kobj = kobj_alloc_directory(dev->name);
	kobj_add_regular(dev->kobj, "playback-volume", audio_read_playback_volume, audio_write_playback_volume, audio);
	kobj_add_regular(dev->kobj, "capture-volume", audio_read_capture_volume, audio_write_capture_volume, audio);

	if(!register_device(dev))
	{
		kobj_remove_self(dev->kobj);
		free(dev->name);
		free(dev);
		return NULL;
	}
	return dev;
}

void unregister_audio(struct audio_t * audio)
{
	struct device_t * dev;

	if(audio && audio->name)
	{
		dev = search_device(audio->name, DEVICE_TYPE_AUDIO);
		if(dev && unregister_device(dev))
		{
			kobj_remove_self(dev->kobj);
			free(dev->name);
			free(dev);
		}
	}
}

void audio_playback_start(struct audio_t * audio, enum audio_rate_t rate, enum audio_format_t fmt, int ch)
{
	if(audio && audio->playback_start)
		audio->playback_start(audio, rate, fmt, ch);
}

int audio_playback_write(struct audio_t * audio, void * buf, int len)
{
	if(audio && audio->playback_write)
		return audio->playback_write(audio, buf, len);
	return 0;
}

void audio_playback_stop(struct audio_t * audio)
{
	if(audio && audio->playback_stop)
		audio->playback_stop(audio);
}

int audio_capture_read(struct audio_t * audio, void * buf, int len)
{
	if(audio && audio->capture_read)
		return audio->capture_read(audio, buf, len);
	return 0;
}

void audio_capture_start(struct audio_t * audio, enum audio_rate_t rate, enum audio_format_t fmt, int ch)
{
	if(audio && audio->capture_start)
		audio->capture_start(audio, rate, fmt, ch);
}

void audio_capture_stop(struct audio_t * audio)
{
	if(audio && audio->capture_stop)
		audio->capture_stop(audio);
}

static int audio_soundpool_mixer(struct audio_t * audio, void * buf, int count)
{
	struct sound_t * pos, * n;
	irq_flags_t flags;
	char * pbuf = buf;
	int32_t left[240];
	int32_t right[240];
	int32_t result[240];
	int32_t * pl = left;
	int32_t * pr = right;
	int16_t * p;
	int bytes = 0;
	int sample;
	int length;
	int empty;
	int i;

	spin_lock_irqsave(&audio->soundpool.lock, flags);
	empty = list_empty_careful(&audio->soundpool.list);
	spin_unlock_irqrestore(&audio->soundpool.lock, flags);
	if(!empty)
	{
		while(count > 0)
		{
			sample = min((int)(count >> 2), 240);
			length = sample << 2;
			memset(left, 0, length);
			memset(right, 0, length);
			spin_lock_irqsave(&audio->soundpool.lock, flags);
			list_for_each_entry_safe(pos, n, &audio->soundpool.list, list)
			{
				if(pos->loop != 0)
				{
					for(i = 0; i < sample; i++)
					{
						if(pos->sample > pos->postion)
						{
							p = (int16_t *)(&pos->source[pos->postion]);
							left[i] += (p[0] * pos->lvol) >> 12;
							right[i] += (p[1] * pos->rvol) >> 12;
							pos->postion++;
						}
						else
						{
							if(pos->loop > 0)
								pos->loop--;
							if(pos->loop != 0)
							{
								pos->postion = 0;
								p = (int16_t *)(&pos->source[pos->postion]);
								left[i] += (p[0] * pos->lvol) >> 12;
								right[i] += (p[1] * pos->rvol) >> 12;
							}
						}
					}
				}
			}
			spin_unlock_irqrestore(&audio->soundpool.lock, flags);
			p = (int16_t *)result;
			for(i = 0; i < sample; i++)
			{
				*p++ = clamp(pl[i], -32768, 32767);
				*p++ = clamp(pr[i], -32768, 32767);
			}
			memcpy(pbuf, result, length);
			bytes += length;
			pbuf += length;
			count -= length;
		}
		spin_lock_irqsave(&audio->soundpool.lock, flags);
		list_for_each_entry_safe(pos, n, &audio->soundpool.list, list)
		{
			if(pos->loop == 0)
			{
				list_del(&pos->list);
				if(pos->cb)
					pos->cb(pos);
			}
		}
		spin_unlock_irqrestore(&audio->soundpool.lock, flags);
	}
	return bytes;
}

static void audio_playback_task(struct task_t * task, void * data)
{
	struct audio_t * audio = (struct audio_t *)data;
	irq_flags_t flags;

	audio_playback_start(audio, AUDIO_RATE_48000, AUDIO_FORMAT_S16, 2);
	while(1)
	{
		unsigned char buf[1024];
		int len = audio_soundpool_mixer(audio, buf, sizeof(buf));
		if(len > 0)
		{
			int l = 0;
			do {
				l += audio_playback_write(audio, buf + l, len - l);
				task_yield();
			} while(l < len);
		}
		else
			break;
	}
	audio_playback_stop(audio);

	spin_lock_irqsave(&audio->soundpool.lock, flags);
	audio->soundpool.playback = NULL;
	spin_unlock_irqrestore(&audio->soundpool.lock, flags);
}

void audio_playback(struct audio_t * audio, struct sound_t * snd)
{
	if(audio && audio_playback_is_support(audio))
	{
		if(snd)
		{
			struct sound_t * pos, * n;
			irq_flags_t flags;
			int found = 0;

			spin_lock_irqsave(&audio->soundpool.lock, flags);
			list_for_each_entry_safe(pos, n, &audio->soundpool.list, list)
			{
				if(pos == snd)
				{
					found = 1;
					break;
				}
			}
			spin_unlock_irqrestore(&audio->soundpool.lock, flags);
			if(!found)
			{
				spin_lock_irqsave(&audio->soundpool.lock, flags);
				list_add_tail(&snd->list, &audio->soundpool.list);
				spin_unlock_irqrestore(&audio->soundpool.lock, flags);
			}
		}
		if(!audio->soundpool.playback)
		{
			irq_flags_t flags;
			char name[256];

			spin_lock_irqsave(&audio->soundpool.lock, flags);
			snprintf(name, sizeof(name), "playback(%s)", audio->name);
			audio->soundpool.playback = task_create(NULL, name, NULL, NULL, audio_playback_task, audio, 0, 0);
			spin_unlock_irqrestore(&audio->soundpool.lock, flags);
		}
	}
}
