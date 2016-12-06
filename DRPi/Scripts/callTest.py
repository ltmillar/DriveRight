import subprocess

date = "16-12-05 21:33:35"
out = subprocess.call(["./setClock.sh", date])
print out
