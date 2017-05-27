#!/usr/bin/env python
#
# Collects readings from several I/O pins.
#

import os

pinread="/usr/share/rpi-monitor/collectors/read-pin"

print 'Board {}'.format(os.popen('uptime --pretty').read().strip())
print 'Heat {}'.format(os.popen('vcgencmd measure_temp').read().strip())
print 'BCM17: {}'.format(os.popen('{} 17'.format(pinread)).read().strip())
print 'BCM27: {}'.format(os.popen('{} 27'.format(pinread)).read().strip())
print 'BCM5 : {}'.format(os.popen('{} 5'.format(pinread)).read().strip())
print 'BCM6 : {}'.format(os.popen('{} 6'.format(pinread)).read().strip())
print 'BCM26: {}'.format(os.popen('{} 26'.format(pinread)).read().strip())
