clock_hz = 20971520
baud_rate = 115200

print("Clock: %.2f Hz" % clock_hz)
print("Desired baud rate: %d" % baud_rate)

print("SBR = %.3f / (OSR + 1)" % (clock_hz / baud_rate))

for i in range(3, 32):
    OSR = i
    SBR = clock_hz / ((OSR + 1) * baud_rate)
    computed_baud = clock_hz / ( (OSR + 1) * SBR)
    print("OSR = %d, SBR = %d, \t Baud rate = %.2f" % (OSR, SBR, computed_baud))