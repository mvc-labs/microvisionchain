Sample init scripts and service configuration for mvcd
==========================================================

1. Service User
---------------------------------

All three Linux startup configurations assume the existence of a "mvc" user
and group.  They must be created before attempting to use these scripts.
The OS X configuration assumes mvcd will be set up for the current user.

2. Configuration
---------------------------------

At a bare minimum, mvcd requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, mvcd will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that mvcd and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If mvcd is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running mvcd without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `contrib/debian/examples/mvc.conf`.

3. Paths
---------------------------------

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/mvcd`  
Configuration file:  `/etc/mvc/mvc.conf`  
Data directory:      `/var/lib/mvcd`  
PID file:            `/var/run/mvcd/mvcd.pid` (OpenRC and Upstart) or `/var/lib/mvcd/mvcd.pid` (systemd)  
Lock file:           `/var/lock/subsys/mvcd` (CentOS)  

The configuration file, PID directory (if applicable) and data directory
should all be owned by the mvc user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
mvc user and group.  Access to mvc-cli and other mvcd rpc clients
can then be controlled by group membership.

4. Installing Service Configuration
-----------------------------------

4a) systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start mvcd` and to enable for system startup run
`systemctl enable mvcd`

4b) OpenRC

Rename mvcd.openrc to mvcd and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/mvcd start` and configure it to run on startup with
`rc-update add mvcd`

4c) Upstart (for Debian/Ubuntu based distributions)

Drop mvcd.conf in /etc/init.  Test by running `service mvcd start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

4d) CentOS

Copy mvcd.init to /etc/init.d/mvcd. Test by running `service mvcd start`.

Using this script, you can adjust the path and flags to the mvcd program by
setting the mvcD and FLAGS environment variables in the file
/etc/sysconfig/mvcd. You can also use the DAEMONOPTS environment variable here.

5. Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
