#! /bin/bash

version_maybe_snapshot=$1
version=$(echo $version_maybe_snapshot | sed -e "s/-SNAPSHOT//")

major_version=$(echo $version | cut --delimiter=. --field=1)
minor_version=$(echo $version | cut --delimiter=. --field=2)
patch_version=$(echo $version | cut --delimiter=. --field=3)

#previous_major_version=0
#previous_minor_version=0
#previous_patch_version=0
#for tag in $(git tag --list '*.*.*' --sort=v:refname); do
#  current_major_version=$(echo $tag | cut --delimiter=. --field=1)
#  current_minor_version=$(echo $tag | cut --delimiter=. --field=2)
#  current_patch_version=$(echo $tag | cut --delimiter=. --field=3)
#
#  if [[ $major_version -gt $current_major_version ]] || \
#     [[ $major_version -eq $current_major_version && $minor_version -gt $current_minor_version ]] || \
#     [[ $major_version -eq $current_major_version && $minor_version -eq $current_minor_version && $patch_version -gt $current_patch_version ]]
#     then
#        if [[ $current_major_version -gt $previous_major_version ]] || \
#           [[ $current_major_version -eq $previous_major_version && $current_minor_version -gt $previous_minor_version ]] || \
#           [[ $current_major_version -eq $previous_major_version && $current_minor_version -eq $previous_minor_version && $current_patch_version -gt $previous_patch_version ]]
#           then
#           previous_major_version=$current_major_version
#           previous_minor_version=$current_minor_version
#           previous_patch_version=$current_patch_version
#           echo $tag
#        fi
#  fi
#done


version_maybe_snapshot=$1
version=$(echo $version_maybe_snapshot | sed -e "s/-SNAPSHOT//")
major_version=$(echo $version | cut --delimiter=. --field=1)
minor_version=$(echo $version | cut --delimiter=. --field=2)
patch_version=$(echo $version | cut --delimiter=. --field=3)

for tag in $(git tag --list '*.*.*' --sort=version:refname); do
  current_major_version=$(echo $tag | cut --delimiter=. --field=1)
  current_minor_version=$(echo $tag | cut --delimiter=. --field=2)
  current_patch_version=$(echo $tag | cut --delimiter=. --field=3)

  if [[ $current_major_version -gt $major_version ]] || \
     [[ $current_major_version -eq $major_version && $current_minor_version -gt $minor_version ]] || \
     [[ $current_major_version -eq $major_version && $current_minor_version -eq $minor_version && $current_patch_version -gt $patch_version ]] || \
     [[ $current_major_version -eq $major_version && $current_minor_version -eq $minor_version && $current_patch_version -eq $patch_version ]]
     then break
     else previous_tag=$tag
  fi
done

echo $previous_tag
