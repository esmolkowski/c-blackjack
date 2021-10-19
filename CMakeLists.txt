cmake_minimum_required(VERSION 3.12)

# set the project name
project(blackjack VERSION 1.0.0)

#configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the executable
add_executable(blackjack main.c renderer.c cards.c game.c)
