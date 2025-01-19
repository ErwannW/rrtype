sounds = {
    Loading_Song = {
        path = "/Sounds/03.Hangar.mp3",
        volume = 20,
        loop = false
    },
    credit_song = {
        path = "/Sounds/credit.mp3",
        volume = 80,
        loop = false
    },
    gameOverBmg = {
        path = "/Sounds/gameOverBMG.mp3",
        volume = 80,
        loop = false
    },
    roomSet = {
        path = "/Sounds/roomSet.mp3",
        volume = 80,
        loop = false
    },
    Level1Sound = {
        path = "/Sounds/stage01.mp3",
        volume = 100,
        loop = false
    },
    Level2Sound = {
        path = "/Sounds/stage02.mp3",
        volume = 100,
        loop = false
    },
    Level3Sound = {
        path = "/Sounds/stage03.mp3",
        volume = 100,
        loop = false
    },
    Level4Sound = {
        path = "/Sounds/stage04.mp3",
        volume = 100,
        loop = false
    },
    button_1_hover = {
        path = "/Sounds/button-hover.mp3",
        volume = 100,
        loop = false
    },
    game_win = {
        path = "/Sounds/19.Mission Completed.mp3",
        volume = 40,
        loop = false
    },
    game_loos = {
        path = "/Sounds/20.Game Over.mp3",
        volume = 40,
        loop = false
    },
    laser_type_1 = {
        path = "/Sounds/Laser Gun1.mp3",
        volume = 100,
        loop = false
    },
    laser_type_2 = {
        path = "/Sounds/Laser Gun2.mp3",
        volume = 100,
        loop = false
    },
    laser_type_3 = {
        path = "/Sounds/Laser Gun3.mp3",
        volume = 100,
        loop = false
    },
    explosion_1 = {
        path = "/Sounds/explosion_1.mp3",
        volume = 100,
        loop = false
    },
    TekkenSongMenu = {
        path = "/Tekken/Songs/Menu.mp3",
        volume = 100,
        loop = true
    },

    TekkenSongStage = {
        path = "/Tekken/Songs/Stage.mp3",
        volume = 100,
        loop = true
    }
}

function getSoundPath(soundName)
  return sounds[soundName].path
end

function getSoundVolume(soundName)
  return sounds[soundName].volume
end

function getSoundLoop(soundName)
  return sounds[soundName].loop
end
