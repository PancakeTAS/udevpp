#include "monitor.hpp"
#include "device.hpp"

#include <stdexcept>
#include <sys/select.h>

using namespace Udev;


Monitor::Monitor(struct udev_monitor* mon, bool ref) : mon(mon) {
    if (!this->mon)
        throw std::invalid_argument("Invalid udev monitor");
    if (ref)
        this->mon = udev_monitor_ref(mon);
    this->fd = udev_monitor_get_fd(this->mon);
    if (this->fd < 0)
        throw std::runtime_error("Failed to get file descriptor");
}

Monitor::Monitor(struct udev_monitor* dev) : Monitor(dev, true) {}


struct udev_monitor* Monitor::get() {
    return udev_monitor_ref(this->mon);
}

struct udev_monitor* Monitor::_get() {
    return this->mon;
}


void Monitor::filterSubsystemDevtype(std::string_view subsystem, std::optional<std::string_view> devtype) {
    const char* devtype_str = devtype ? devtype->data() : NULL;
    if (udev_monitor_filter_add_match_subsystem_devtype(this->mon, subsystem.data(), devtype_str) < 0)
        throw std::runtime_error("Failed to add filter");
}

void Monitor::filterTag(std::string_view tag) {
    if (udev_monitor_filter_add_match_tag(this->mon, tag.data()) < 0)
        throw std::runtime_error("Failed to add filter");
}


void Monitor::updateFilter() {
    if (udev_monitor_filter_update(this->mon) < 0)
        throw std::runtime_error("Failed to update filter");
}

void Monitor::removeFilter() {
    if (udev_monitor_filter_remove(this->mon) < 0)
        throw std::runtime_error("Failed to remove filter");
}


void Monitor::enableReceiving() {
    if (udev_monitor_enable_receiving(this->mon) < 0)
        throw std::runtime_error("Failed to enable receiving");
}

void Monitor::setReceiveBufferSize(int size) {
    if (udev_monitor_set_receive_buffer_size(this->mon, size) < 0)
        throw std::runtime_error("Failed to set receive buffer size");
}


bool Monitor::canReceive() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(this->fd, &fds);

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int ret = select(this->fd + 1, &fds, NULL, NULL, &tv);
    if (ret < 0)
        throw std::runtime_error("Failed to select");
    return ret > 0 && FD_ISSET(this->fd, &fds);
}

Device Monitor::receive() {
    struct udev_device* dev = udev_monitor_receive_device(this->mon);
    if (!dev)
        throw std::runtime_error("Failed to receive device");
    return Device(dev, false);
}


Monitor::~Monitor() {
    udev_monitor_unref(this->mon);
}
