import argparse
import csv
import time
import thermoflex as tf


def sample_voltage(nodes, duration=10, interval=0.1, out_file='voltage_readings.csv'):
    start = time.time()
    with open(out_file, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        header = ['timestamp', 'node_id', 'supply_voltage']
        writer.writerow(header)
        while time.time() - start < duration:
            tf.update()  # run network background tasks
            for net in tf.controls.NodeNet.netlist:
                for node in net.node_list:
                    node.status('compact')
                    status = node.node_status.get('volt_supply')
                    writer.writerow([time.time(), ''.join(f'{b:02X}' for b in node.node_id), status])
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

    sample_voltage(nets, args.duration, args.interval, args.outfile)
    tf.endAll()


if __name__ == '__main__':
    main()
