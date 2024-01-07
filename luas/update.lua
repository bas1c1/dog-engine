scen = mainScene()
obj = scen:getObject(1)
rb = obj:get_rigidbody2D()

if getKey(88) == -32768 then
	rb.gravity = 9.81
elseif getKey(67) == -32768 then
	rb.gravity = -9.81
end