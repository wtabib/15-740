import os



result = []

m =  [5,10, 15, 20]
for j in m:
    f = "pthread_12%s7.txt" % j
    os.system('rm '+f)

    os.system(f);
    for i in range(100):
        os.system('sudo nice -n -20 ./read_heavy_workload.o 1 2 %s 7 >> pthread_12%s7.txt' % (j, j));
    #f.close()

    fi = open(f);

    tot = 0.0
    for i in fi:
        tot = tot + int(i)

    result.append(tot/1000)


for i,k in enumerate(m):
    print m[i],result[i]




