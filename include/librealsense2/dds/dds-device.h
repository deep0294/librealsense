// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2022 Intel Corporation. All Rights Reserved.

#pragma once

#include "dds-defines.h"
#include <librealsense2/h/rs_internal.h>

#include <memory>
#include <functional>
#include <string>

namespace librealsense {
namespace dds {


namespace topics {
class device_info;
}  // namespace topics


class dds_participant;


// Represents a device via the DDS system. Such a device exists as of its identification by the device-watcher, and
// always contains a device-info.
// 
// The device may not be ready for use (will not contain sensors, profiles, etc.) until it is "run".
//
class dds_device
{
public:
    static std::shared_ptr< dds_device > find( dds_guid const & guid );

    static std::shared_ptr< dds_device > create( std::shared_ptr< dds_participant > const & participant,
                                                 dds_guid const & guid,
                                                 topics::device_info const & info );

    topics::device_info const & device_info() const;

    bool is_running() const;

    // Make the device ready for use. This may take time! Better to do it in the background...
    void run();

    //----------- below this line, a device must be running!

    size_t num_of_sensors() const;

    size_t foreach_sensor( std::function< void( const std::string& name ) > fn ) const;

    size_t foreach_video_profile( size_t sensor_index, std::function< void( const rs2_video_stream& profile, bool def_prof ) > fn ) const;
    size_t foreach_motion_profile( size_t sensor_index, std::function< void( const rs2_motion_stream& profile, bool def_prof ) > fn ) const;

private:
    class impl;
    std::shared_ptr< impl > _impl;

    // Ctor is private: use find() or create() instead. Same for dtor -- it should be automatic
    dds_device( std::shared_ptr< impl > );
    ~dds_device();
};  // class dds_device


}  // namespace dds
}  // namespace librealsense
