#include "hawktracer/base_timeline.h"
#include "hawktracer/alloc.h"

#include "internal/timeline_klass.hpp"
#include "internal/mutex.h"

#include <cstring>
#include <cassert>

void
ht_base_timeline_init(HT_BaseTimeline* timeline, va_list args)
{
    size_t buffer_capacity = 1024;
    HT_Boolean thread_safe = HT_TRUE;
    HT_Boolean serialize_events = HT_FALSE;

    const char* label = va_arg(args, const char*);;

    while (label != nullptr)
    {
        if (strncmp("buffer-capacity", label, 15) == 0)
        {
            buffer_capacity = va_arg(args, size_t);
        }
        else if (strncmp("thread-safe", label, 11) == 0)
        {
            thread_safe = va_arg(args, HT_Boolean);
        }
        else if (strncmp("serialize-events", label, 16) == 0)
        {
            serialize_events = va_arg(args, HT_Boolean);
        }
        else
        {
            va_arg(args, void*);
        }
        label = va_arg(args, const char*);
    }

    timeline->buffer_usage = 0;
    timeline->buffer_capacity = buffer_capacity;
    timeline->buffer = (HT_Byte*)ht_alloc(buffer_capacity);
    timeline->id_provider = ht_event_id_provider_get_default();
    timeline->serialize_events = serialize_events;

    timeline->listeners = (timeline->klass->listeners == NULL) ?
                ht_timeline_listener_container_create() : timeline->klass->listeners;

    timeline->locking_policy = thread_safe ? ht_mutex_create() : NULL;
}

void
ht_base_timeline_deinit(HT_BaseTimeline* timeline)
{
    assert(timeline);

    ht_free(timeline->buffer);

    if (timeline->klass->listeners == NULL)
    {
        ht_timeline_listener_container_destroy(timeline->listeners);
    }

    if (timeline->locking_policy)
    {
        ht_mutex_destroy(timeline->locking_policy);
    }
}