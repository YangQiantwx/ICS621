from subprocess import check_output, CalledProcessError 
import sys
#
partitions = [1000,10000,100000,1000000,10000000,100000000]

times = []
for x in partitions:
    timesb = []
    timesf = []
    timesp = []
    for i in range(5):
        try:
            timesb.append(check_output(["./bh", str(x)]))
            timesf.append(check_output(["./fh", str(x)]))
            timesp.append(check_output(["./ph", str(x)]))
            print("."),
            print(timesb)
            print(timesf)
            print(timesp)

        except CalledProcessError as e:
            print(e.output)
    
    print(str(x))
    sys.stdout.flush()
