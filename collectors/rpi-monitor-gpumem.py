#!/usr/bin/env python
#
# Collects base GPU/ARM memory usage.
#

import os

free_disk='df -h / --output=avail --no-sync | sed 1d'

print 'Board {}'.format(os.popen('uptime --pretty').read().strip())
print 'Linux: {}'.format(os.popen('uname -r').read().strip())
print 'Heat {}'.format(os.popen('vcgencmd measure_temp').read().strip())
print 'Allocated {}'.format(os.popen('vcgencmd get_mem arm').read().strip())
print 'Allocated {}'.format(os.popen('vcgencmd get_mem gpu').read().strip())
print 'GPU mem {}'.format(os.popen('vcgencmd get_mem reloc').read().strip())
print 'Disk Free = {}'.format(os.popen(free_disk).read().strip())
