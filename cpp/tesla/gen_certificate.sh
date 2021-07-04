#!/bin/bash

RED=`tput setaf 1`
GRN=`tput setaf 2`
NON=`tput sgr0`

prefix_server=certi_server
prefix_client=certi_client

if [ "$#" -eq 1 ]
then
	prefix_server=$1
fi

if [ "$#" -eq 2 ]
then
	prefix_client=$1
fi

#cwd=$PWD/stage/certificate
cwd=../stage/certificate
mkdir -p ${cwd}

if [[ ! -f ${cwd}/${prefix_server}.crt || ! -f ${cwd}/${prefix_server}.key || ! -f ${cwd}/${prefix_server}.csr || ! -f ${cwd}/${prefix_server}.dh2048 ]]
then
  echo -e ${RED}Generate openssl server certificate...${NON}
  openssl genrsa -out ${cwd}/${prefix_server}.key 2048
  openssl req -new -key ${cwd}/${prefix_server}.key -out ${cwd}/${prefix_server}.csr -subj "/C=KR/ST=Gyeonggi-do/L=Suwon-si/O=jaej/CN=localhost"
  openssl x509 -req -days 365 -in ${cwd}/${prefix_server}.csr -signkey ${cwd}/${prefix_server}.key -out ${cwd}/${prefix_server}.crt
  openssl genpkey -genparam -algorithm DH -out ${cwd}/${prefix_server}.dh2048 -pkeyopt dh_paramgen_prime_len:2048
fi


if [[ ! -f ${cwd}/${prefix_client}.crt || ! -f ${cwd}/${prefix_client}.key || ! -f ${cwd}/${prefix_client}.csr ]]
then
  echo -e ${RED}Generate openssl client certificate...${NON}
  openssl genrsa -out ${cwd}/${prefix_client}.key 2048
  openssl req -new -key ${cwd}/${prefix_client}.key -out ${cwd}/${prefix_client}.csr -subj "/C=KR/ST=Gyeonggi-do/L=Suwon-si/O=jaej/CN=localhost"
  openssl x509 -req -days 365 -in ${cwd}/${prefix_client}.csr -signkey ${cwd}/${prefix_client}.key -out ${cwd}/${prefix_client}.crt
fi

echo -e ${GRN}Verify openssl certificate...${NON}
openssl verify -CAfile ${cwd}/${prefix_server}.crt ${cwd}/${prefix_server}.crt
openssl verify -CAfile ${cwd}/${prefix_client}.crt ${cwd}/${prefix_client}.crt

echo -e ${GRN}Completed openssl certificate...${NON}
