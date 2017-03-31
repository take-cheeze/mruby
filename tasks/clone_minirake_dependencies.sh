#!/usr/bin/env bash

set -e
# set -o xtrace

cd $(dirname $0)

CLONE_DIR=$1
CLONE_OPTS="--depth 1"

echo "CLONE_DIR=${CLONE_DIR}"

mkdir -p ${CLONE_DIR}
if [ -d ${CLONE_DIR}/mgem-list ] ; then
    cd ${CLONE_DIR}/mgem-list
    git pull origin master
    cd $(dirname $0)
else
    git clone ${CLONE_OPTS} git@github.com:mruby/mgem-list.git ${CLONE_DIR}/mgem-list
fi

for gem in $(cat ../mrbgems/mruby-bin-minirake/.dep_gems.txt); do
    # skip core mrbgem
    if [ -d ../mrbgems/${gem} ] ; then
        continue
    fi

    echo "cloning: ${gem}"

    # skip if already cloned
    if [ -d ${CLONE_DIR}/${gem} ] ; then
        continue
    fi

    GEM_INFO=${CLONE_DIR}/mgem-list/${gem}.gem
    if [ -f ${GEM_INFO} ] ; then
        GEM_REPO_URL=$(cat ${GEM_INFO} | grep '^repository:' | sed -e 's/^repository: \([^ ]*\).*/\1/')
    fi
    GEM_CLONE_DIR=${CLONE_DIR}/${gem}

    if git clone ${CLONE_OPTS} -b minirake "git@github.com:mrbgems/${gem}.git" "${GEM_CLONE_DIR}" ; then
        echo ''
    else
        rm -rf "${GEM_CLONE_DIR}"
        git clone ${CLONE_OPTS} "${GEM_REPO_URL}" "${GEM_CLONE_DIR}"
    fi
done
