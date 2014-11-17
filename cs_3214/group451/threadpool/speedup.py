import sys
import time
import subprocess

verbose = False
stats = []

def main():
    file_name = "speedup_test_log"
    cores = []
    for idx, arg in enumerate(sys.argv):
        if idx != 0:
            cores.append(int(arg))
            file_name += "_" + arg

    file_name += ".txt"

    run_trials(cores, file_name)



def run_trials(core_trials, file_name):
    with open(file_name, "a") as file:
        file.write("-------------------------------------------------------" +
            "-------------------------\n\n")
        file.write("Start Time: " + time.strftime("%H:%M:%S") + "\n")
    for cores in core_trials:
        with open(file_name, "a") as file:
                file.write(str(cores) + " threads\n")
        sys.stdout.flush()
        # depth, base time, para time
        R1 = [-1, 8, 0, 0]
        depth_trials = []
        if cores > 2:
            depth_trials = range(cores - 2, 25)
        else:
            depth_trials = range(1, 25)
        print "Running test with", cores, "threads and testing dephts", str(depth_trials)
        trials_results = []
        latest = []

        # Test each depth
        for depth in depth_trials:
            # Run quicksort
            result = run_quicksort(depth, cores, False)
            # write to file
            with open(file_name, "a") as file:
                file.write("Cores: " + str(result[0]) +
                    "\tDepth: " + str(result[1]) +
                    "\tSerial: " + str(result[2]) +
                    "\tParallel: " + str(result[3]) + "\n")
            trials_results.append(result)
            # Add latest run
            if len(latest) >= 3:
                latest.pop(0)
            latest.append(result[3])
            # Check if lastest runs are increasing in time
            if len(latest) > 2 and latest[0] < latest[1] and latest[1] < latest[2]:
                print "Stop testing depths"
                break

        # Get best depth
        lowest_time = 99999
        best_index = -1
        for idx, trial in enumerate(trials_results):
            if trial[3] < lowest_time:
                lowest_time = trial[3]
                best_index = idx
        print "Best depth:", trials_results[best_index][1], "\tTime:", lowest_time

        R1 = trials_results[best_index]
        R2 = run_quicksort(R1[1], R1[0], False)
        R3 = run_quicksort(R1[1], R1[0], True)

        # Get serial running time
        base_time = R3[2]
        SU1 = base_time/R1[3]
        SU2 = base_time/R2[3]
        SU3 = base_time/R3[3]

        avg_su = (SU1 + SU2 + SU3) / 3

        with open(file_name, "a") as file:
            file.write("\nBest runs:\n")
            file.write(format(R1, SU1))
            file.write(format(R2, SU2))
            file.write(format(R3, SU3))
            file.write("\nAverage speedup: " + str(avg_su) + "\n")
    with open(file_name, "a") as file:
        file.write("End Time: " + time.strftime("%H:%M:%S") + "\n")

def format(result, speedup):
    result_str = "Cores: " + str(result[0]) + "\tDepth: " + str(result[1]) + "\tSerial: " + str(result[2]) + "\tParallel: " + str(result[3]) + "\tSpeeup: " + str(speedup) + "\n"
    return result_str

def run_quicksort(depth, cores, serial):
    if serial:
        args = ('./quicksort', '-s', '42', '-d', str(depth), '-n', str(cores), '300000000')
    else:
        args = ('./quicksort', '-s', '42', '-d', str(depth), '-n', str(cores), '-q', '300000000')

    program_open = subprocess.Popen(args, stdout=subprocess.PIPE)
    program_open.wait()

    output = program_open.stdout.read();
    # print output

    program_output = output.split("\n")
    base_time = 0
    para_time = 0
    if serial:
        base_time = float(' '.join(program_output[0].split()).split()[3])
        para_time = float(' '.join(program_output[2].split()).split()[3])
        print program_output[0].strip('\n')
        print program_output[1].strip('\n')
        print program_output[2].strip('\n')
        print ""
    else:
        para_time = float(' '.join(program_output[1].split()).split()[3])
        print program_output[0].strip('\n')
        print program_output[1].strip('\n')
        print ""

    return [cores, depth, base_time, para_time]


main()
