import argparse
import csv
import time
import thermoflex as tf


def run_current_steps(nodes, setpoints, hold_time=1.0, interval=0.1, outfile="current_pulse.csv"):
    with open(outfile, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["timestamp", "node_id", "muscle", "setpoint", "load_amps"])

        # Configure muscles to current mode and enable
        for node in nodes:
            for muscle in node.muscles.values():
                muscle.setMode("amps")
                muscle.setEnable(True)

        for target in setpoints:
            step_start = time.time()
            for node in nodes:
                for muscle in node.muscles.values():
                    muscle.setSetpoint(conmode="amps", setpoint=target)

            last_read = {}
            while time.time() - step_start < hold_time:
                for node in nodes:
                    node.status("compact")
                    for key, muscle in node.muscles.items():
                        amps = muscle.SMA_status['load_amps'][0] if muscle.SMA_status['load_amps'] else None
                        nid = ".".join(str(b) for b in node.id)
                        writer.writerow([time.time(), nid, key, target, amps])
                        last_read[(nid, key)] = amps
                time.sleep(interval)

            for (nid, key), amps in last_read.items():
                if amps is not None and abs(amps - target) > 0.2:
                    print(f"Warning: Node {nid} muscle {key} target {target}A, measured {amps:.2f}A")

        for node in nodes:
            node.disableAll()


def main():
    parser = argparse.ArgumentParser(description="Run stepped current commands and verify feedback")
    parser.add_argument("--currents", default="0.5,1.0", help="comma separated current setpoints in amps")
    parser.add_argument("--hold", type=float, default=1.0, help="time to hold each setpoint in seconds")
    parser.add_argument("--interval", type=float, default=0.1, help="sample interval in seconds")
    parser.add_argument("--outfile", default="current_pulse.csv", help="CSV output file")
    args = parser.parse_args()

    setpoints = [float(c) for c in args.currents.split(",") if c]

    nets = tf.discover([105])
    for net in nets:
        net.refreshDevices()

    node_list = [node for net in nets for node in net.node_list]
    run_current_steps(node_list, setpoints, args.hold, args.interval, args.outfile)
    tf.endAll()


if __name__ == '__main__':
    main()
