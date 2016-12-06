import subprocess

date = "16-12-05 23:00:00"
out = subprocess.call(["./setClock.sh", date])
print out
