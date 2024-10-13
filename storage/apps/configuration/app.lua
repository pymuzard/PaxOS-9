local gui = paxo.gui
local settings = paxo.system.settings
local color = paxo.color

local oldWin
local slider, lstSleepTime

local sleepInSeconds={10000, 30000, 60000, 120000, 180000, 300000, 600000}

function paxo.update()
end

function paxo.load()

    initMainScreen()
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
    local passwd = gui.keyboard("connect ","passwd")
    settings:connectWifi(SSID, passwd)
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
--  GESTION ECRAN COULEURQ
-- -----------------------------------


function initColorScreen()

    local winColor = manageWindow()



    

end




function manageWindow()

    local win = gui.window.new()
    gui.setWindow(win)

    if oldWin then 
        gui.del(oldWin) 
        oldWin  =nil 
    end
    
    oldWin = win
    return win
end