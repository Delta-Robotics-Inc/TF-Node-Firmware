import argparse
import csv
import time
import thermoflex as tf


def run_current_pulse(nodes, target_amp, pulse_time=2.0, duration=10.0, interval=0.1, outfile='current_pulse.csv'):
    start = time.time()
    with open(outfile, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['timestamp', 'node_id', 'muscle', 'load_amps'])

        # Configure muscles
        for net in nodes:
            for node in net.node_list:
                for musc in node.muscles.values():
                    node.setMode('amps', f'm{musc.idnum+1}')
                    node.setSetpoint(tf.command_t.modedef.index('amps'), f'm{musc.idnum+1}', target_amp)
                    node.enable(musc)

        while time.time() - start < duration:
            tf.update()
            for net in tf.controls.NodeNet.netlist:
                for node in net.node_list:
                    node.status('compact')
                    for key, muscle in node.muscles.items():
                        amps = muscle.SMA_status['load_amps'][0] if muscle.SMA_status['load_amps'] else None
                        writer.writerow([time.time(), ''.join(f'{b:02X}' for b in node.node_id), key, amps])
            if time.time() - start > pulse_time:
                for net in nodes:
                    for node in net.node_list:
                        node.disableAll()
            time.sleep(interval)

        for net in nodes:
            for node in net.node_list:
                node.disableAll()


def main():
    parser = argparse.ArgumentParser(description='Run a current pulse through the muscle')
    parser.add_argument('--target', type=float, required=True, help='target current in amps')
    parser.add_argument('--pulse', type=float, default=2.0, help='pulse time in seconds')
    parser.add_argument('--duration', type=float, default=10.0)
    parser.add_argument('--interval', type=float, default=0.1)
    parser.add_argument('--outfile', default='current_pulse.csv')
    args = parser.parse_args()

    nets = tf.discover([105])
    for net in nets:
        net.refreshDevices()

    run_current_pulse(nets, args.target, args.pulse, args.duration, args.interval, args.outfile)
    tf.endAll()


if __name__ == '__main__':
    main()
