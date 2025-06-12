import argparse
import csv
import time
import thermoflex as tf


def sample_current(nodes, duration=10.0, interval=0.1, outfile="no_load_current.csv"):
    start = time.time()
    with open(outfile, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["timestamp", "node_id", "muscle", "load_amps"])
        while time.time() - start < duration:
            for node in nodes:
                node.status("compact")
                for key, muscle in node.muscles.items():
                    amps = muscle.SMA_status['load_amps'][0] if muscle.SMA_status['load_amps'] else None
                    nid = ".".join(str(b) for b in node.id)
                    writer.writerow([time.time(), nid, key, amps])
            time.sleep(interval)


def main():
    parser = argparse.ArgumentParser(description='Measure no-load current')
    parser.add_argument('--duration', type=float, default=10.0)
    parser.add_argument('--interval', type=float, default=0.1)
    parser.add_argument('--outfile', default='no_load_current.csv')
    args = parser.parse_args()

    nets = tf.discover([105])
    for net in nets:
        net.refreshDevices()

    node_list = [node for net in nets for node in net.node_list]
    sample_current(node_list, args.duration, args.interval, args.outfile)
    tf.endAll()


if __name__ == '__main__':
    main()
