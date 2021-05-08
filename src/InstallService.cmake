message("-- Installing: /lib/systemd/system/phoenix-killer.service")
file(WRITE "/lib/systemd/system/phoenix-killer.service"
        "[Unit]
Description=Phoenix Killer service

[Service]
ExecStart=${CMAKE_INSTALL_PREFIX}/bin/PhoenixKiller --with-md5-check --cleanup

[Install]
WantedBy=multi-user.target")

message("-- Installing: /lib/systemd/system/phoenix-killer.timer")
file(WRITE "/lib/systemd/system/phoenix-killer.timer"
        "[Unit]
Description=Run Phoenix Killer hourly and on boot

[Timer]
OnBootSec=15min
OnUnitInactiveSec=1h
Persistent=true
Unit=phoenix-killer.service

[Install]
WantedBy=timers.target")

message("-- systemctl daemon-reload")
execute_process(COMMAND systemctl daemon-reload)
message("-- Start phoenix-killer.service")
execute_process(COMMAND systemctl start phoenix-killer.service)
message("-- Enable phoenix-killer.timer")
execute_process(COMMAND systemctl enable phoenix-killer.timer)
message("-- Start phoenix-killer.timer")
execute_process(COMMAND systemctl start phoenix-killer.timer)