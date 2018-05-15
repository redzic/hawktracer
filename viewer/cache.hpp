#ifndef HAWKTRACER_VIEWER_CACHE_HPP
#define HAWKTRACER_VIEWER_CACHE_HPP

#include <hawktracer/base_types.h>
#include <hawktracer/parser/event.hpp>

namespace HawkTracer
{
namespace viewer
{

using EventRef = std::reference_wrapper<const parser::Event>;

class Cache
{
public:
    bool range_in_cache(HT_TimestampNs start_ts,
                        HT_TimestampNs stop_ts,
                        HT_EventKlassId klass_id);

    bool get_data(HT_EventKlassId klass_id, std::vector<EventRef>& events);

    void update(HT_EventKlassId klass_id,
                std::vector<EventRef> new_response);

    void insert_event(EventRef event);
    
private: 
    std::unordered_map<HT_EventKlassId, std::vector<EventRef>> _events;
};

} // namespace viewer
} // namespace HawkTracer

#endif // HAWKTRACER_VIEWER_CACHE_HPP
