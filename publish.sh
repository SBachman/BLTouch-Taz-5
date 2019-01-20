# A helper script file that updates the FW version number, tags and pushes it.

# Get the current version number
THEIR_VERSION=`grep -m1 "#define SHORT_BUILD_VERSION " Marlin/Version.h | cut -d \" -f 2`
LULZ_VERSION=`grep -m1 "#define LULZBOT_FW_VERSION " Marlin/Conditionals_LulzBot.h | cut -d \" -f 2 | cut -d . -f 2`
NEXT_VERSION=`expr $LULZ_VERSION + 1`

echo "Increment version to $NEXT_VERSION?"
select yn in "Yes" "No"; do
  case $yn in
    Yes)
      sed -i "s/#define LULZBOT_FW_VERSION \".[0-9]\\+\"/#define LULZBOT_FW_VERSION \".$NEXT_VERSION\"/" Marlin/Conditionals_LulzBot.h
      git add Marlin/Conditionals_LulzBot.h
      git commit -m "Change version number" -m "Changed version number to $THEIR_VERSION.$NEXT_VERSION"
      git tag v$THEIR_VERSION.$NEXT_VERSION HEAD
      git push origin v$THEIR_VERSION.$NEXT_VERSION
      break;;
    No)
      exit;;
  esac
done

git push

