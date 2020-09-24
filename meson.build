project(
    'phosphor-hostPowerEvent-monitor',
    'cpp',
    version: '1.0',
    default_options: [
        'cpp_std=c++17',
    ],
)

executable(
    'hostPowerEvent-monitor',
    [
        'hostPowerEventMonitor.cpp',
    ],
    dependencies: [
        dependency('phosphor-logging'),
        dependency('sdbusplus'),
        dependency('phosphor-dbus-interfaces'),
        dependency('sdeventplus'),
        dependency('threads'),
    ],
    install: true,
    install_dir: get_option('bindir')
)

install_data(sources : 'bmc_hostPowerEvent_config.json', install_dir : '/etc/hostPowerEventMon')

conf_data = configuration_data()
conf_data.set('HOST_POWER_EVENT_CONFIG_FILE', '"/etc/hostPowerEventMon/bmc_hostPowerEvent_config.json"')
conf_data.set('HOST_POWER_EVENT_BUS_NAME', '"xyz.openbmc_project.HostPowerEventMon"')
conf_data.set('HOST_POWER_EVENT_SENSOR_PATH', '"/xyz/openbmc_project/sensors/oem/"')
conf_data.set('CURRENT_HOST_STATE', '"CurrentHostState"')
conf_data.set('REQUEST_HOST_TRANSITION', '"RequestedHostTransition"')
conf_data.set('RESTART_CAUSE', '"RestartCause"')

configure_file(output : 'config.h',
               configuration : conf_data)

systemd = dependency('systemd')
conf_data = configuration_data()
conf_data.set('bindir', get_option('prefix') / get_option('bindir'))
configure_file(
  input: 'phosphor-hostPowerEvent-monitor.service.in',
  output:'phosphor-hostPowerEvent-monitor.service',
  configuration: conf_data,
  install: true,
  install_dir: systemd.get_pkgconfig_variable('systemdsystemunitdir'))
