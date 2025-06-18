import argparse
import csv
import time
import thermoflex as tf


def sample_voltage(nodes, duration=10.0, interval=0.1, out_file="voltage_readings.csv"):
    start = time.time()
    with open(out_file, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["timestamp", "node_id", "supply_voltage"])
        while time.time() - start < duration:
            for node in nodes:
                node.status("compact")
                v = node.node_status.get("volt_supply")
                nid = ".".join(str(b) for b in node.id)
                writer.writerow([time.time(), nid, v])
            time.sleep(interval)


def main():
    parser = argparse.ArgumentParser(description='Log node supply voltage over time')
    parser.add_argument('--duration', type=float, default=10.0, help='logging duration in seconds')
    parser.add_argument('--interval', type=float, default=0.1, help='sample interval in seconds')
    parser.add_argument('--outfile', default='voltage_readings.csv', help='CSV output file')
    args = parser.parse_args()

    nets = tf.discover([105])
    for net in nets:
        net.refreshDevices()

    node_list = [node for net in nets for node in net.node_list]
    sample_voltage(node_list, args.duration, args.interval, args.outfile)
    tf.endAll()


if __name__ == '__main__':
    main()
