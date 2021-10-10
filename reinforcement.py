import subprocess, sys, os

def test():
    for i in range(4):
        print(i)
        cp = subprocess.run(['make run-build'],
                            check=True,
                            shell=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT,
                            universal_newlines=True)
        print(cp.stdout)

def main():
    test()

if __name__ in "__main__":
    main()