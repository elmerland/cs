import os, time
import argparse

letters = ["a", "b", "c", "d"]
matrix_sizes = [1056, 2112, 4224]
num_procs_list = [1, 2, 4, 8, 16, 24, 32]

argparser = argparse.ArgumentParser(description="Use random sampling to find a motif.")
argparser.add_argument("-l", "--letter", dest="prob_letter", type=str, help="The problem letter bitch!")
argparser.add_argument("-s", "--start", dest="start", default=1, type=int, help="Start index of processes")
argparser.add_argument("-e", "--end", dest="end", default=32, type=int, help="End index of processes")
args = argparser.parse_args()

prob_letter = args.prob_letter
p_start     = num_procs_list.index(args.start)
p_end       = num_procs_list.index(args.end) + 1

for letter in letters:
	for num_proc in num_procs_list[p_start:p_end]:
		for size in matrix_sizes:

			output_file = " >> results/strong_full_list_t8.txt"

			p = "-p " + str(num_proc)
			n = "-n " + str(size)
			t = ""
			a = ""
			o = ""

			if letter == "b":
				a = "-a"
			elif letter in ["c", "d"]:
				if letter == "d":
					o = "-o"
				a = "-a"
				t = "-t 8"

			echo="echo 'Running letter: {0} N: {1} P: {2} T: {3}' {4}".format(letter, str(size), str(p), t, output_file)
			os.system(echo)
			cmd="./runner.sh -g -e {0} {1} {2} {3} {4}".format(str(p), str(n), str(t), str(a), str(o))
			cmd += output_file
			print(cmd)
			os.system(cmd)
			time.sleep(1)
		print()
		
