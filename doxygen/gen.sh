#!/bin/bash
set -e

if [ ! $# == 5 ]; then
  echo "usage : sh gen.sh 产品名 版本 输入待分析目录 输出文档目录 doxyfile模板文件"
  echo "example : sh gen.sh taihang 1.0.0.0 code doc doxyfile.tpl"
  exit 0
fi

PRODUCT_NAME=$1
FULL_VERSION_NUM=$2
INPUT_DIRECTORY=$3
OUTPUT_DIRECTORY=$4
DOXYFILE=$5

apt-get install -y doxygen
sed -i "s#%productName%#${PRODUCT_NAME}#g" ${DOXYFILE}
sed -i "s#%fullVersionNum%#${FULL_VERSION_NUM}#g" ${DOXYFILE}
sed -i "s#%inputDirectory%#${INPUT_DIRECTORY}#g" ${DOXYFILE}
sed -i "s#%outputDirectory%#${OUTPUT_DIRECTORY}#g" ${DOXYFILE}
doxygen ${DOXYFILE}
