#! /bin/bash

vulnerabilites=$(gh api \
  -H "Accept: application/vnd.github+json" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  -X GET \
  -F severity=critical,high \
  -F state=open \
  /repos/marc-adaptive/forked-aeron/dependabot/alerts)

exit $(echo $vulnerabilites | jq length)


exit $(gh api -H "Accept: application/vnd.github+json" -H "X-GitHub-Api-Version: 2022-11-28" -X GET -F severity=critical,high -F state=open /repos/marc-adaptive/forked-aeron/dependabot/alerts | jq length)

#echo $vulnerabilities | jq '.[]' | jq length
#
#for vulnerability in $(echo $vulnerabilites | jq '.[]' | jq -r '.state'); do
#  exit 1
#done
#
#exit 0