local gui = paxo.gui
local settings = paxo.system.settings
local color = paxo.color

local oldWin
local slider, lstSleepTime

local groupRadio
local radioText,radioBackground, radioBorder
local textColor, backgroundColor, borderColor
local sliderR, sliderG, sliderB


local sleepInSeconds={10000, 30000, 60000, 120000, 180000, 300000, 600000}

function paxo.update()
end

function paxo.load()
    initMainScreen()
end



function int(x)
    return math.floor(x)
end


-- -----------------------------------
--  GESTION ECRAN PRINCIPAL
-- -----------------------------------

function initMainScreen()

    local win = manageWindow()

    -- Titre
    local lblLogo = gui.image.new(win, "/apps/configuration/logo.png", 10, 10, 55, 60, color.white)

    local title = gui.label.new(win, 65, 45, 320, 40)
    title:setText("arametres")
    title:setTextColor(97, 183, 157)
    title:setFontSize(30)

    -- Version de l'OS
    lblOSVersion = gui.label.new(win, 20, 100, 300, 20)
    lblOSVersion:setText("OS Version: "..settings:getOSVersion())

    -- gestion de la luminosité
    local lblBrightness = gui.label.new(win, 20, 140, 320, 20)
    lblBrightness:setText("Luminosité")

    local brightness = settings:getBrightness()

    slider = gui.slider.new(win, 20, 165, 200, 20, 0, 100, brightness)
    slider:setFormatPercentage(true)
    slider:displayValue(true)
    slider:onChange(changeBrightness)

    -- Mode Veille
    local lblActivationVeille = gui.label.new(win, 20, 225, 160, 20)
    lblActivationVeille:setText("Activation Veille")

    local sleepTime = settings:getStandBySleepTime()

    lstSleepTime = gui.vlist.new(win, 170, 210, 100, 50)
    lstSleepTime:setSelectionColor(color.toColor(97, 183, 157))
    lstSleepTime:setAutoSelect(true)
    lstSleepTime:setSpaceLine(2)
    lstSleepTime:setAutoSelect(true)
    lstSleepTime:setSelectionFocus(gui.focus.center)

    for i,k in ipairs(sleepInSeconds) do
        local lblSleepValue = gui.label.new (lstSleepTime, 0, 0, 100, 16)
        lblSleepValue:setHorizontalAlignment(gui.alignment.center)
        lblSleepValue:setVerticalAlignment(gui.alignment.center)
        lblSleepValue:setText(displayTime(k))
    end

    for i,k in ipairs(sleepInSeconds) do
        if k == sleepTime then 
            lstSleepTime:select(i-1)
            break
        end
    end

    -- 300
    local lblWifi = gui.label.new(win, 20, 290, 100, 20)
    lblWifi:setText("Wifi")

    local connectedWifi = settings:getConnectedWifi()
    local lstSSID = settings:getAvailableWifiSSID()

    local lstAvailableWifi = gui.vlist.new(win, 100, 270, 170, 60)
    lstAvailableWifi:setSelectionColor(color.toColor(97, 183, 157))
    lstAvailableWifi:setAutoSelect(true)
    lstAvailableWifi:setSpaceLine(2)
    lstAvailableWifi:setSelectionFocus(gui.focus.center)


    for i,k in ipairs(lstSSID) do
        local lblWifiSSID = gui.label.new (lstAvailableWifi, 0, 0, 170, 16)
        lblWifiSSID:setHorizontalAlignment(gui.alignment.center)
        lblWifiSSID:setVerticalAlignment(gui.alignment.center)
        lblWifiSSID:setText(k)
        lblWifiSSID:onClick(function() connectWifi(k) end )
    end

    for i,k in ipairs(lstSSID) do
        if k == connectedWifi then 
            lstAvailableWifi:select(i-1)
            break
        end
    end

    local lblColor = gui.label.new(win, 20, 350, 100, 20)
    lblColor:setText("Couleurs")

    local lblColorTest = gui.label.new(win, 150, 350, 100, 40)
    lblColorTest:setText("Test")
    lblColorTest:onClick(initColorScreen)

    local colorBackground = settings:getBackgroundColor()
    local colorBorder = settings:getBorderColor()
    local colorText = settings:getTextColor()

    lblColorTest:setBackgroundColor(colorBackground)
    lblColorTest:setBorderColor(colorBorder)
    lblColorTest:setBorderSize(1)
    lblColorTest:setTextColor(colorText)
    lblColorTest:setVerticalAlignment(gui.alignment.center)
    lblColorTest:setHorizontalAlignment(gui.alignment.center)


    --Bouton Sauvegarder
    lblSauvergarder = gui.label.new(win, 50, 420, 200, 30)
    lblSauvergarder:setRadius(15)
    lblSauvergarder:setBorderSize(1)
    lblSauvergarder:setBorderColor(color.black)
    lblSauvergarder:setText("Sauvegarder")
    lblSauvergarder:setHorizontalAlignment(gui.alignment.center)
    lblSauvergarder:setVerticalAlignment(gui.alignment.center)
    lblSauvergarder:onClick(saveChanges)
end


function connectWifi(SSID)
    local passwd = gui.keyboard("connect "..SSID,"passwd")
    if passwd ~= "passwd" then
        settings.connectWifi(SSID, passwd)
    end
end


function displayTime(millis)
    local min = math.floor((millis/1000) / 60)
    local sec = (millis/1000) % 60

    if min > 0 then result = tostring(min).." min" end
    if min > 0 and sec >0 then result = " " end
    if sec > 0 then result = string.format("%.0f",sec).. " sec" end

    return result

end

function changeBrightness()
    --local value = slider:getValue()
    settings.setBrightness(slider:getValue(), false)
end

function saveChanges()
    -- save Brightness
    --local value = slider:getValue()
    settings.setBrightness(slider:getValue(), true)

    -- save Veille
    settings.setStandBySleepTime(sleepInSeconds[lstSleepTime:getSelected()+1], true)

    paxo.app.quit()
end



-- -----------------------------------
--  GESTION ECRAN COULEURS
-- -----------------------------------

local lblTestCouleur

function initColorScreen()

    local winColor = manageWindow()

    local btnBack = gui.image.new(winColor, "/apps/configuration/back.png",10, 20, 20, 20, color.white)
    btnBack:onClick(initMainScreen)

    local title = gui.label.new(winColor, 40, 20, 280, 40)
    title:setText("Couleurs")
    title:setTextColor(97, 183, 157)
    title:setFontSize(30)

    local topRadio = 80

    -- Gestion des boutons radios
    radioText = gui.radio.new(winColor, 20, topRadio)
    local lblRadioText = gui.label.new(winColor, 45, topRadio, 50, 20)
    lblRadioText:setText("Texte")

    radioBackground = gui.radio.new(winColor, 100, topRadio)
    local lblRadioBackground = gui.label.new(winColor, 125, topRadio, 90, 20)
    lblRadioBackground:setText("Background")

    radioBorder = gui.radio.new(winColor, 220, topRadio)
    local lblRadioBorder = gui.label.new(winColor, 245, topRadio, 50, 20)
    lblRadioBorder:setText("Border")

    groupRadio = {radioText,radioBackground, radioBorder }
    radioText:onClick(function () selectRadio(groupRadio, radioText) end)
    radioBackground:onClick(function () selectRadio(groupRadio, radioBackground) end)
    radioBorder:onClick(function () selectRadio(groupRadio, radioBorder) end)

    -- radioText:setState(true)

    -- Gestin grille de couleurs
    local colors = {color.success, color.warning, color.error, color.white, color.black, color.red, color.green, color.blue, color.yellow, color.grey, color.magenta, color.cyan, color.violet, color.orange, color.pink, color.lightOrange, color.lightGreen, color.lightBlue, color.lightGrey, color.greenPaxo }

    -- Label Test Couleurs
    lblTestCouleur = gui.label.new(winColor, 250, 370, 50, 60)
    lblTestCouleur:setText("Test")
    lblTestCouleur:setBorderSize(1)
    lblTestCouleur:setHorizontalAlignment(gui.alignment.center)
    lblTestCouleur:setVerticalAlignment(gui.alignment.center)

    textColor = settings:getTextColor()
    backgroundColor = settings:getBackgroundColor()
    borderColor = settings:getBorderColor()

    lblTestCouleur:setTextColor(textColor)
    lblTestCouleur:setBackgroundColor(backgroundColor)
    lblTestCouleur:setBorderColor(borderColor)


    local width = 300
    local height = 200
    local gridRow = 4
    local gridCol = 5


    local grille = gui.canvas.new(winColor, 10, 140, width, height)
    grille:onTouch(
        function (coord)
            local x = int(coord.x / (width/gridCol))
            local y = int(coord.y / (height/gridRow))

            local selectedColor = color.white 
            if y*gridCol+x <= #colors then
                selectedColor = colors[y*gridCol+x +1]
            end
            print("onTouch color = "..tostring(selectedColor))
            setPredefinedColor(selectedColor)
        end
    )

    -- Création de la grille de couleur
    for j=0,gridRow-1 do 
        for i=0,gridCol-1 do 
            local couleur = color.white
            if j*gridCol+i < #colors then
                couleur = colors[j*gridCol+i +1]
            end

            grille:fillRect(int(i * width/gridCol)+1, int(j* height / gridRow )+1, int(width/gridCol )-2 , int(height/gridRow)-2, couleur)

        end
    end

    -- slider Rouge
    sliderR = gui.slider.new(winColor, 20, 370, 150, 20, 0, 255, 100)
    sliderR:setValueColor(color.red)
    sliderR:onChange(setRGBColor)

    -- slider Green
    sliderG = gui.slider.new(winColor, 20, 400, 150, 20, 0, 255, 100)
    sliderG:setValueColor(color.green)
    sliderG:onChange(setRGBColor)

    -- Slider Blue
    sliderB = gui.slider.new(winColor, 20, 430, 150, 20, 0, 255, 100)
    sliderB:setValueColor(color.blue)
    sliderB:onChange(setRGBColor)

    selectRadio(groupRadio, radioText)

end

function setRGBColor()

    local couleur =color.toColor(sliderR:getValue(),sliderG:getValue(),sliderB:getValue() )
    if radioText:getState() then 
        textColor = couleur
        lblTestCouleur:setTextColor(textColor)
    elseif radioBackground:getState() then
        backgroundColor = couleur
        lblTestCouleur:setBackgroundColor(backgroundColor)
    elseif radioBorder:getState() then
        borderColor = couleur
        lblTestCouleur:setBorderColor(borderColor)
    else
        return
    end

end

function setPredefinedColor(col)
    -- print("setPredefinedColor")

    if radioText:getState() then 
        textColor = col
        lblTestCouleur:setTextColor(textColor)
    elseif radioBackground:getState() then
        backgroundColor = col
        lblTestCouleur:setBackgroundColor(backgroundColor)
    elseif radioBorder:getState() then
        borderColor = col
        lblTestCouleur:setBorderColor(borderColor)
    else
        return
    end
        
    local selectedColor = table.pack(color.toRGB(col))
    if selectedColor.n ==3 then
        sliderR:setValue(selectedColor[1])
        sliderG:setValue(selectedColor[2])
        sliderB:setValue(selectedColor[3])
    end

end




function saveColor()

    settings:setBackgroundColor(backgroundColor, true)
    settings:setTextColor(textColor, true)
    settings:setBorderColor(borderColor, true)

    initMainScreen()

end



-- select the radio button, and unselect the other ones
-- adjust the sliders values based on the rigth color
function selectRadio(groupRadio, radio)
    for i,r in ipairs(groupRadio) do
        r:setState(r == radio)        
    end
    
    local selectedColor
    if radio == radioBackground then
        selectedColor = table.pack(color.toRGB(backgroundColor))
    elseif radio == radioBorder then
        selectedColor = table.pack(color.toRGB(borderColor))
    elseif radio == radioText then
        selectedColor = table.pack(color.toRGB(textColor))
    else
        return
    end
    if selectedColor.n ==3 then
        sliderR:setValue(selectedColor[1])
        sliderG:setValue(selectedColor[2])
        sliderB:setValue(selectedColor[3])
    end


end


function manageWindow()

    local win = gui.window.new()
    gui.setWindow(win)

    if oldWin ~= nil then 
        -- gui.del(oldWin) 
        oldWin  =nil 
    end
    
    oldWin = win
    return win
end