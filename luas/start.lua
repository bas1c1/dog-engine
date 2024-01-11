mainSc = mainScene()

gameObjectNew = object.new("obj1", 1, objectType.BASE)
tr = gameObjectNew:add_transform(800, 200, 0, 0, objectType.BASE)

sp = sprite.new("sprites/800x200.png")
sp2 = sprite.new(" ")
gameObjectNew:add_animation()
gameObjectNew:get_animation():bindTransform(tr)
gameObjectNew:get_animation():addSprite(spriteRef(sp))
gameObjectNew:get_animation():addSprite(spriteRef(sp2))
gameObjectNew:get_animation().delay = 30

bc = gameObjectNew:add_box_collider2D()
gameObjectNew:get_box_collider2D():bindTransform(tr)
gameObjectNew:get_box_collider2D():bindScene(mainSc)

rb = gameObjectNew:add_rigidbody2D()
gameObjectNew:get_rigidbody2D():bindTransform(tr)
gameObjectNew:get_rigidbody2D():bindScene(mainSc)

mainSc:addObject(objectRef(gameObjectNew))

print("engine started omg")
