#include <nlohmann/json.hpp>
#include <sdbusplus/bus.hpp>

#include <string>

namespace phosphor
{
namespace hostPowerEvent
{

class HostPowerEventMon
{
  public:
    //HostPowerEventMon() = delete;
    HostPowerEventMon(const HostPowerEventMon&) = delete;
    HostPowerEventMon& operator=(const HostPowerEventMon&) = delete;
    HostPowerEventMon(HostPowerEventMon&&) = delete;
    HostPowerEventMon& operator=(HostPowerEventMon&&) = delete;
    virtual ~HostPowerEventMon() = default;

    /** @brief Constructs EventMo
     */
    HostPowerEventMon(std::vector<std::string> sensors)
    {
        createHostPowerEventSensors(sensors);
    }

    /** @brief Create sensors for hostPowerEvent monitoring */
    void createHostPowerEventSensors(std::vector<std::string> sensors);

};

} // namespace hostPowerEvent
} // namespace phosphor
