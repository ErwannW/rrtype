fonts = {
    button_1 = {
        path = "/Fonts/Natural-Mono-Regular.ttf",
        size = 35.0,
        color = "#ffffff",
        padding_left = 0,
        padding_top = -0.7
    },
    input_1 = {
        path = "/Fonts/Natural-Mono-Regular.ttf",
        size = 25.0,
        color = "#1C00A6",
        padding_left = 0,
        padding_top = -0.7
    },
    Title = {
        path = "/Fonts/Natural-Mono-Regular.ttf",
        size = 50,
        color = "#ffffff",
        padding_left = 0,
        padding_top = 0
    },
    Title_red = {
        path = "/Fonts/Natural-Mono-Regular.ttf",
        size = 50,
        color = "#ff0000",
        padding_left = 0,
        padding_top = 0
    },
    Title_blue = {
        path = "/Fonts/Natural-Mono-Regular.ttf",
        size = 50,
        color = "#1C00A6",
        padding_left = 0,
        padding_top = 0
    },
    Version = {
        path = "/Fonts/Natural-Mono-Regular.ttf",
        size = 15,
        color = "#ffffff",
        padding_left = 0,
        padding_top = 0
    }
}

function getFontSize(fontName)
    return fonts[fontName].size
end

function getFontColor(fontName)
    return fonts[fontName].color
end

function getFontPaddingTop(fontName)
    if fonts[fontName] and fonts[fontName].padding_top then
        return fonts[fontName].padding_top
    else
        return 0
    end
end

function getFontPaddingLeft(fontName)
    if fonts[fontName] and fonts[fontName].padding_left then
        return fonts[fontName].padding_left
    else
        return 0
    end
end

function getFontPath(fontName)
    return fonts[fontName].path
end