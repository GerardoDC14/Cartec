import os

### curl 
os.system('apt update')
os.system('apt upgrade -y')
os.system('apt install curl -y')

##pip
os.system('curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py')
os.system('python3 get-pip.py')
os.system('rm -rf get-pip.py')

##unzip
os.system('apt-get update -y')
os.system('apt-get install -y unzip')

##github
os.system('apt install git-all -y')

##nano
os.system('apt install nano')