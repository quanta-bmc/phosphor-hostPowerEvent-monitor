#include "config.h"
#include "hostPowerEventMonitor.hpp"
#include <phosphor-logging/log.hpp>

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>

namespace phosphor
{
namespace hostPowerEvent
{

using namespace phosphor::logging;

void monitorHostStateService(){

    std::cout << "#### monitorHostStateService started\n";
    boost::asio::io_context io;
    auto conn= std::make_shared<sdbusplus::asio::connection>(io);
    static auto match = sdbusplus::bus::match::match(
    *conn,
    "type='signal',member='PropertiesChanged', "
    "path='/xyz/openbmc_project/state/host0', "
    "arg0='xyz.openbmc_project.State.Host'",
    [](sdbusplus::message::message& message) {
        std::string intfName;
        std::map<std::string, std::variant<std::string>> properties;
        std::string value;
        try
        {
            message.read(intfName, properties);
            if (properties.empty())
            {
                std::cerr << "ERROR: Empty PropertiesChanged signal received\n";
                return;
            }
            else
            {
                if (properties.begin()->first == "CurrentHostState")
                {
                    value = std::get<std::string>(properties.begin()->second);
                    std::cout << "CurrentHostState value:" << value <<  "\n";

                    boost::asio::io_context io;
                    auto conn= std::make_shared<sdbusplus::asio::connection>(io);
                    conn->async_method_call(
                    [](boost::system::error_code ec) {
                        if (ec)
                        {
                            std::cerr << "failed to set Value action\n";
                        }else 
                            std::cout << "async call to Properties. Set serialized via yield OK!\n";
                     },
                    HOST_POWER_EVENT_BUS_NAME,
                    "/xyz/openbmc_project/sensors/oem/CurrentHostState",
                    "org.freedesktop.DBus.Properties", "Set",
                    "xyz.openbmc_project.Sensor.Value", "Value", std::variant<std::string>{value});
                }
                else if (properties.begin()->first == "RequestedHostTransition")
                {
                    value = std::get<std::string>(properties.begin()->second);
                    std::cout << "RequestedHostTransition value:" << value <<  "\n";

                    boost::asio::io_context io;
                    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
                    conn->async_method_call(
                    [](boost::system::error_code ec) {
                        if (ec)
                        {
                            std::cerr << "failed to set Value action\n";
                        }else 
                            std::cout << "async call to Properties. Set serialized via yield OK!\n";
                     },
                    HOST_POWER_EVENT_BUS_NAME,
                    "/xyz/openbmc_project/sensors/oem/RequestedHostTransition",
                    "org.freedesktop.DBus.Properties", "Set",
                    "xyz.openbmc_project.Sensor.Value", "Value", std::variant<std::string>{value});
                }
            }     

        }
        catch (std::exception& e)
        {
            std::cerr << "Unable to read host state\n";
            return;
        }
        std::cout << "#### monitorHostStateService property change end\n";       
    });

}

void monitorRestartCauseService(){

    std::cout << "#### monitorRestartCauseService started\n";
    boost::asio::io_context io;
    auto conn= std::make_shared<sdbusplus::asio::connection>(io);
    static auto match = sdbusplus::bus::match::match(
    *conn,
    "type='signal',member='PropertiesChanged', "
    "path='/xyz/openbmc_project/control/host0/restart_cause', "
    "arg0='xyz.openbmc_project.Control.Host.RestartCause'",
    [](sdbusplus::message::message& message) {
        std::string intfName;
        std::map<std::string, std::variant<std::string>> properties;
        std::string value;
        try
        {
            message.read(intfName, properties);
            if (properties.empty())
            {
                std::cerr << "ERROR: Empty PropertiesChanged signal received\n";
                return;
            }
            else
            {
                if (properties.begin()->first == "RestartCause")
                {
                    value = std::get<std::string>(properties.begin()->second);
                    std::cout << "RestartCause value:" << value <<  "\n";

                    boost::asio::io_context io;
                    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
                    conn->async_method_call(
                    [](boost::system::error_code ec) {
                        if (ec)
                        {
                            std::cerr << "failed to set Value action\n";
                        }else 
                            std::cout << "async call to Properties. Set serialized via yield OK!\n";
                     },
                    HOST_POWER_EVENT_BUS_NAME,
                    "/xyz/openbmc_project/sensors/oem/RestartCause",
                    "org.freedesktop.DBus.Properties", "Set",
                    "xyz.openbmc_project.Sensor.Value", "Value", std::variant<std::string>{value});
                }
            }     

        }
        catch (std::exception& e)
        {
            std::cerr << "Unable to read host state\n";
            return;
        }
        std::cout << "#### monitorRestartCauseService property change end\n";       
    });

}

void HostPowerEventMon::createHostPowerEventSensors(std::vector<std::string> sensors)
{

    boost::asio::io_context io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name(HOST_POWER_EVENT_BUS_NAME);
    auto server = sdbusplus::asio::object_server(conn);
    std::string ifaceobjpath = "";
    //std::vector<std::string> sensorConfigs = {CURRENT_HOST_STATE, REQUEST_HOST_TRANSITION, RESTART_CAUSE};

    for (auto& name : sensors)
    {
        ifaceobjpath = HOST_POWER_EVENT_SENSOR_PATH + name;
        std::shared_ptr<sdbusplus::asio::dbus_interface> iface = server.add_interface(ifaceobjpath, "xyz.openbmc_project.Sensor.Value");     
        iface->register_property("Value", std::string("n/a"), sdbusplus::asio::PropertyPermission::readWrite);
        iface->register_property("Unit", std::string("xyz.openbmc_project.Sensor.Value.Unit.Percent"), sdbusplus::asio::PropertyPermission::readWrite);
        // no Unit for OEM, so use Percent temperarily
        iface->initialize();
    }

    monitorHostStateService();
    monitorRestartCauseService();

    io.run();
}


} // namespace hostPowerEvent
} // namespace phosphor

/**
 * @brief Main
 */
int main()
{

    std::vector<std::string> sensorNames = {CURRENT_HOST_STATE, REQUEST_HOST_TRANSITION, RESTART_CAUSE};
    phosphor::hostPowerEvent::HostPowerEventMon HostPowerEventMon(sensorNames);
    
    return 0;    
}
