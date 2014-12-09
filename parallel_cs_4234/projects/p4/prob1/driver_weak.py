import os, time

letters = ["a", "b", "c", "d"]
num_procs_list = [1, 2, 4, 8, 16, 24, 32]
matrix_sizes = [[480, 768, 1056], [960, 1536, 2112], [960, 1536, 2112], [1920, 3072, 4224], [1920, 3072, 4224], [3840, 6144, 8448], [3840, 6144, 8448]]

for letter in letters:
  for idx, proc in enumerate(num_procs_list):
    for size in matrix_sizes[idx]:
      output_file = " >> results/weak_full_list_t8.txt"

      p = "-p " + str(proc)
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
