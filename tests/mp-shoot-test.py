import subprocess
from multiprocessing import Process

shooter1 = ['../build/shoot-cam1']
shooter2 = ['../build/shoot-cam2']

def capture(shooter, shoots = 10):
    for s in range(shoots):
        print "\tExecuting '" + shooter + "'..."
        subprocess.Popen(shooter).communicate()


def parallelize():
    procs = []

    procs.append(Process(None, capture, None, (shooter1)))
    procs.append(Process(None, capture, None, (shooter2)))

    for p in range(len(procs)):
        print "Starting p"+str(p)+"..."
        procs[p].start()

    for p in procs:
        p.join()

parallelize()

