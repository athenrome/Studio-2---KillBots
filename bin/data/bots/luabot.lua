initData = {}
angle = 0

function init(data)
	initData = data
	return {health = 1.0, motor = 1.0, weaponSpeed = 1.0, weaponStrength = 1.0}
	print("YAY")
end

function step(input, output)
	output.motor = 0
	output.moveDirection = {x=-1, y=-1}
	output.action="scan"
	if #input.scanResult == 0 then
		output.lookDirection = {x=math.cos(angle), y=math.sin(angle)}
		angle = angle + 0.1
	end
	for i,s in ipairs(input.scanResult) do
		if s.type=="robot" then
			output.action="shoot"
			x = s.position.x
			y = s.position.y
			x = x - input.position.x
			y = y - input.position.y
			output.lookDirection.x = x
			output.lookDirection.y = y
		end
	end
	return output
end
