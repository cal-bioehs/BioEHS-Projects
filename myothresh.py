import serial #Imports PySerial for communication with Arduino

def closest_centroid(val, centroids):
    return min(centroids, key=lambda x: abs(x - val))

def group_by_first(pairs):
    keys = []
    for key, _ in pairs:
        if key not in keys:
            keys.append(key)
    return [[y for x, y in pairs if x == key] for key in keys]

def group_by_centroid(vals, centroids):
    pairs = [[closest_centroid(val, centroids), val] for val in vals]
    return group_by_first(pairs)

def find_centroid(cluster):
    """ Given a cluster, returns the centroid (the average) of all vals in cluster. """
    return float(sum([val for val in cluster]))/len(cluster)

def k_means(vals, k=2, max_updates=100):
    old_centroids = []
    n = 0
    centroids = sorted(list(set(vals)))[:k]
    while old_centroids != centroids and n < max_updates:
        old_centroids = centroids
        clusters = group_by_centroid(vals, centroids)
        centroids = sorted([find_centroid(cluster) for cluster in clusters])
        n += 1
    print('Resting, Flexing Clusters: {}'.format(centroids))
    #print(set(vals))
    #print(len(vals))
    print('Ideal Threshold: {}'.format(sum(centroids) * 0.6))

ser = serial.Serial('/dev/cu.usbmodem14241', 9600)
sensorValues = []
while len(sensorValues) < 3000:
    if len(sensorValues) % 1000 == 0:
        print(len(sensorValues))
    sensorValues.append(int(ser.readline().decode('utf-8')))
k_means(sensorValues)
