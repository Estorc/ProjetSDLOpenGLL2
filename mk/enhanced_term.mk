export TERM = xterm-256color

NC=\033[0;0m

BOLD=1
ITALIC=3
UNDELINE=4
BLINK=5

FG_GREY=30
FG_RED=31
FG_GREEN=32
FG_ORANGE=33
FG_BLUE=34
FG_PURPLE=35
FG_AQUA=36
FG_WHITE=37
FG_DEFAULT=39

BG_GREY=40
BG_RED=41
BG_GREEN=42
BG_ORANGE=43
BG_BLUE=44
BG_PURPLE=45
BG_AQUA=46
BG_WHITE=47
BG_DEFAULT=49

NOTE_COL=\033[0;${FG_GREY};${BG_DEFAULT}m
ACT_COL=\033[0;${FG_BLUE};${BG_DEFAULT}m
STEP_COL=\033[0;${FG_DEFAULT};${BG_DEFAULT};${BOLD}m
FILE_COL=\033[0;${FG_ORANGE};${BG_DEFAULT};${ITALIC}m
SUCCESS_COL=\033[0;${FG_GREEN};${BG_DEFAULT};${BOLD}m
ERROR_COL=\033[0;${FG_RED};${BG_DEFAULT};${BOLD}m

PRINT=@printf

NEWLINE := $(shell printf "\n")


define PRINT_SEPARATOR
	WIDTH=$$(tput cols); \
	MESSAGE="$(1)"; \
	PLAIN_MSG=$$(echo "$$MESSAGE" | sed 's/\\033\[[0-9;]*m//g'); \
	MSG_LEN=$$(echo "$$PLAIN_MSG" | wc -c); \
	PAD=$$(( (WIDTH - MSG_LEN - 1) / 2 )); \
	PADDING=$$(printf '=%.0s' $$(seq 1 $$PAD)); \
	printf "${STEP_COL}%s $$MESSAGE ${STEP_COL}%s${NC}\n" "$$PADDING" "$$PADDING";
endef


define PRINT_CENTERED
	WIDTH=$$(tput cols); \
	MESSAGE="$(1)"; \
	PLAIN_MSG=$$(echo "$$MESSAGE" | sed 's/\\033\[[0-9;]*m//g'); \
	MSG_LEN=$$(echo "$$PLAIN_MSG" | wc -c); \
	PAD=$$(( (WIDTH - MSG_LEN - 1) / 2 )); \
	PADDING=$$(printf ' %.0s' $$(seq 1 $$PAD)); \
	printf "%s $$MESSAGE %s${NC}\n" "$$PADDING" "$$PADDING"
endef