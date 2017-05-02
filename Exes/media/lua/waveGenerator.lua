KnightCost = 2
ThiefCost = 7
AssassinCost = 3


function createWave(maxKnapsackDimension,spawnpointList,spawnTime)	

	local timestamp = GetCurrentTimestamp()
--	print("load create wave timestamp"..timestamp)
	math.randomseed( timestamp )
	local knapsackRemaningSpace = maxKnapsackDimension
	idWave = idWave+1

--	print("idWave ="..idWave)
	local wave = Wave(idWave,spawnTime);

	local spNum = 0
	for k, v in pairs( spawnpointList ) do
		spNum = spNum + 1
--	   print(k, v)
	end

--	local spNum = table.getn(spawnpointList)
--	print("spNum ="..spNum)
--	print("1-remaningSpace="..knapsackRemaningSpace)

	repeat
		local currentSP = (math.random(1000) % spNum)+1
--		print("currSP"..currentSP)
--		currentSP = math.floor(currentSP)
--		print("floorSP="..currentSP.."sppoint ="..spawnpointList[currentSP])

--		print("2-remaningSpace="..knapsackRemaningSpace)

		local kCount,aCount,tCount,remaningSpace = fillKnapsack(knapsackRemaningSpace)
		knapsackRemaningSpace = remaningSpace
		print("after fillKanpsack="..kCount.." "..aCount.." "..tCount.." "..knapsackRemaningSpace)

		if kCount>0 then
			wave:AddWaveNode(WaveNode("Knight",kCount,spawnpointList[currentSP]))
		end
		if aCount>0 then
			wave:AddWaveNode(WaveNode("Assassin",aCount,spawnpointList[currentSP]))
		end
		if tCount>0 then
			wave:AddWaveNode(WaveNode("ThiefEnemy",tCount,spawnpointList[currentSP]))
		end
--		print("3-remaningSpace="..knapsackRemaningSpace)
	until knapsackRemaningSpace<=0

	AddWave(wave)
	return idWave
end

function fillKnapsack(kRemaningSpace)

--	print("fillKnapsack space="..kRemaningSpace)
	local currentKnight=0
	local currentAssassin=0
	local currentThief=0

	local maxKnight =  math.floor(kRemaningSpace / KnightCost);
	if maxKnight>0 then
		currentKnight = round(math.random(0,maxKnight))
		kRemaningSpace = kRemaningSpace - (currentKnight * KnightCost)
	end
--	print("knight max="..maxKnight.." current="..currentKnight.." space="..kRemaningSpace)

	local maxAssassin = math.floor(kRemaningSpace / AssassinCost);
	if maxAssassin>0 then
		currentAssassin =round(math.random(0,maxAssassin))
		kRemaningSpace = kRemaningSpace - (currentAssassin * AssassinCost)
	end
--	print("Assassin max="..maxAssassin.." current="..currentAssassin.." space="..kRemaningSpace)


	local maxThief = math.floor(kRemaningSpace / ThiefCost);
	if maxThief>0 then
		currentThief = round(math.random(0,maxThief))
		kRemaningSpace = kRemaningSpace - (currentThief * ThiefCost)
	end
--	print("Thief max="..maxThief.." current="..currentThief.." space="..kRemaningSpace)

	if maxThief<=0 and maxAssassin<=0 and maxKnight<=0 then
		return 0,0,0,-1
	else 
		return currentKnight,currentAssassin,currentThief,kRemaningSpace
	end
end

function round(num)
	   local quant,frac = math.modf(num)
	   if num>=0 then
		   if frac >= 0.5 then
				return quant + 1
			else
				return quant
			end
	   else
		   if frac <= -0.5 then
				return quant - 1
			else
				return quant
			end
	   end

end