#include "device.hpp"

#include <stdexcept>

using namespace Udev;

Device::Device(struct udev_device* dev, bool ref) : dev(dev) {
    if (!this->dev)
        throw std::invalid_argument("Invalid udev device");
    if (ref)
        this->dev = udev_device_ref(dev);
}

Device::Device(struct udev_device* dev) : Device(dev, true) {}


struct udev_device* Device::get() {
    return udev_device_ref(this->dev);
}

struct udev_device* Device::_get() {
    return this->dev;
}


Device Device::getParent() {
    struct udev_device* parent = udev_device_get_parent(this->dev);
    if (!parent)
        throw std::runtime_error("Failed to get parent device");
    return {parent};
}

Device Device::getParentWithSubsystemDevtype(
    const std::string& subsystem, const std::string& devtype) {
    struct udev_device* parent = udev_device_get_parent_with_subsystem_devtype(
        this->dev, subsystem.c_str(), devtype.c_str());
    if (!parent)
        throw std::runtime_error("Failed to get parent device with subsystem and devtype");
    return {parent};
}


#define DEFINE_UDEV_GETTER(method_name, udev_func, error_msg) \
std::string Device::method_name() { \
    const char* val = udev_func(this->dev); \
    if (!val) \
        throw std::runtime_error(error_msg); \
    return std::string(val); \
}

DEFINE_UDEV_GETTER(getDevpath,   udev_device_get_devpath,   "Failed to get device path")
DEFINE_UDEV_GETTER(getSubsystem, udev_device_get_subsystem, "Failed to get device subsystem")
DEFINE_UDEV_GETTER(getDevtype,   udev_device_get_devtype,   "Failed to get device type")
DEFINE_UDEV_GETTER(getSyspath,   udev_device_get_syspath,   "Failed to get device syspath")
DEFINE_UDEV_GETTER(getSysname,   udev_device_get_sysname,   "Failed to get device sysname")
DEFINE_UDEV_GETTER(getSysnum,    udev_device_get_sysnum,    "Failed to get device sysnum")
DEFINE_UDEV_GETTER(getDevnode,   udev_device_get_devnode,   "Failed to get device devnode")
DEFINE_UDEV_GETTER(getDriver,    udev_device_get_driver,    "Failed to get device driver")
DEFINE_UDEV_GETTER(getAction,    udev_device_get_action,    "Failed to get device action")

uint64_t Device::getDevnum() { return udev_device_get_devnum(this->dev); }
uint64_t Device::getSeqnum() { return udev_device_get_seqnum(this->dev); }
uint64_t Device::getUsecSinceInitialized() { return udev_device_get_usec_since_initialized(this->dev); }
bool Device::isInitialized() { return udev_device_get_is_initialized(this->dev); }

std::string Device::getProperty(const std::string& key) {
    const char* val = udev_device_get_property_value(this->dev, key.c_str());
    if (!val)
        throw std::runtime_error("Failed to get property value");
    return {val};
}

std::string Device::getSysattr(const  std::string& sysattr) {
    const char* val = udev_device_get_sysattr_value(this->dev, sysattr.c_str());
    if (!val)
        throw std::runtime_error("Failed to get sysattr value");
    return {val};
}


bool Device::hasTag(const  std::string& tag) {
    return udev_device_has_tag(this->dev, tag.c_str());
}

bool Device::hasCurrentTag(const  std::string& tag) {
    return udev_device_has_current_tag(this->dev, tag.c_str());
}


void Device::setSysattr(const  std::string& sysattr, const  std::string& value) {
    if (udev_device_set_sysattr_value(this->dev, sysattr.c_str(), value.c_str()) < 0)
        throw std::runtime_error("Failed to set sysattr value");
}


std::vector<std::string> Device::getDevlinks() {
    std::vector<std::string> devlinks;
    struct udev_list_entry* devlink = udev_device_get_devlinks_list_entry(this->dev);
    if (!devlink)
        throw std::runtime_error("Failed to get device links");

    struct udev_list_entry* entry = nullptr;
    udev_list_entry_foreach(entry, devlink) {
        const char* val = udev_list_entry_get_name(entry);
        if (val)
            devlinks.emplace_back(val);
    }
    return devlinks;
}

std::vector<std::string> Device::getTags() {
    std::vector<std::string> tags;
    struct udev_list_entry* tag = udev_device_get_tags_list_entry(this->dev);
    if (!tag)
        throw std::runtime_error("Failed to get device tags");

    struct udev_list_entry* entry = nullptr;
    udev_list_entry_foreach(entry, tag) {
        const char* val = udev_list_entry_get_name(entry);
        if (val)
            tags.emplace_back(val);
    }
    return tags;
}

std::vector<std::string> Device::getCurrentTags() {
    std::vector<std::string> current_tags;
    struct udev_list_entry* tag = udev_device_get_current_tags_list_entry(this->dev);
    if (!tag)
        throw std::runtime_error("Failed to get current device tags");

    struct udev_list_entry* entry = nullptr;
    udev_list_entry_foreach(entry, tag) {
        const char* val = udev_list_entry_get_name(entry);
        if (val)
            current_tags.emplace_back(val);
    }
    return current_tags;
}


std::unordered_map<std::string, std::string> Device::getProperties() {
    std::unordered_map<std::string, std::string> properties;
    struct udev_list_entry* prop = udev_device_get_properties_list_entry(this->dev);
    if (!prop)
        throw std::runtime_error("Failed to get device properties");

    struct udev_list_entry* entry = nullptr;
    udev_list_entry_foreach(entry, prop) {
        const char* key = udev_list_entry_get_name(entry);
        const char* value = udev_list_entry_get_value(entry);
        if (key && value)
            properties[key] = value;
    }
    return properties;
}

std::unordered_map<std::string, std::string> Device::getSysattrs() {
    std::unordered_map<std::string, std::string> sysattrs;
    struct udev_list_entry* sysattr = udev_device_get_sysattr_list_entry(this->dev);
    if (!sysattr)
        throw std::runtime_error("Failed to get device sysattrs");

    struct udev_list_entry* entry = nullptr;
    udev_list_entry_foreach(entry, sysattr) {
        const char* key = udev_list_entry_get_name(entry);
        const char* value = udev_list_entry_get_value(entry);
        if (key && value)
            sysattrs[key] = value;
    }
    return sysattrs;
}


Device::~Device() {
    udev_device_unref(this->dev);
}
