#include "context.hpp"
#include "monitor.hpp"

#include <stdexcept>
#include <string>

using namespace Udev;

Context::Context()
    : ctx(udev_new()) {
    if (!this->ctx)
        throw std::runtime_error("Failed to create udev context");
}

Context::Context(struct udev* ctx)
    : ctx(udev_ref(ctx)) {
    if (!this->ctx)
        throw std::invalid_argument("Invalid udev contxt");
}


struct udev* Context::get() {
    return udev_ref(this->ctx);
}

struct udev* Context::_get() {
    return this->ctx;
}


Device Context::getDeviceFromSyspath(const std::string& syspath) {
    struct udev_device* dev = udev_device_new_from_syspath(this->ctx, syspath.c_str());
    if (!dev)
        throw std::runtime_error("Failed to create device from syspath");
    return {dev, false};
}

Device Context::getDeviceFromDevnum(DevnumType type, uint64_t devnum) {
    char ctype = (type == BLOCK) ? 'b' : 'c';
    struct udev_device* dev = udev_device_new_from_devnum(this->ctx, ctype, devnum);
    if (!dev)
        throw std::runtime_error("Failed to create device from devnum");
    return {dev, false};
}

Device Context::getDeviceFromSubsystemSysname(const std::string& subsystem, const std::string& sysname) {
    struct udev_device* dev = udev_device_new_from_subsystem_sysname(this->ctx, subsystem.c_str(), sysname.c_str());
    if (!dev)
        throw std::runtime_error("Failed to create device from subsystem and sysname");
    return {dev, false};
}

Device Context::getDeviceFromId(const std::string& id) {
    struct udev_device* dev = udev_device_new_from_device_id(this->ctx, id.c_str());
    if (!dev)
        throw std::runtime_error("Failed to create device from device id");
    return {dev, false};
}

Device Context::getDeviceFromEnv() {
    struct udev_device* dev = udev_device_new_from_environment(this->ctx);
    if (!dev)
        throw std::runtime_error("Failed to create device from environment");
    return {dev, false};
}


Monitor Context::createMonitor(MonitorType type) {
    struct udev_monitor* mon = udev_monitor_new_from_netlink(this->ctx, type == MonitorType::UDEV ? "udev" : "kernel");
    if (!mon)
        throw std::runtime_error("Failed to create udev monitor");
    return {mon, false};
}


Context::~Context() {
    udev_unref(this->ctx);
}
