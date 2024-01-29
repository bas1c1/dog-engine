mainSc = mainScene()

gameObjectNew = object.new("obj1", 1, objectType.BASE)
tr = gameObjectNew:add_transform(800, 200, 0, 0, objectType.BASE)

sp = sprite.new("sprites/800x200.png")
sp2 = sprite.new(" ")
gameObjectNew:add_spriteRender()
gameObjectNew:get_spriteRender():bindTransform(tr)
gameObjectNew:get_spriteRender():bindSprite(spriteRef(sp))

bc = gameObjectNew:add_box_collider2D()
gameObjectNew:get_box_collider2D():bindTransform(tr)
gameObjectNew:get_box_collider2D():bindScene(mainSc)

rb = gameObjectNew:add_rigidbody2D()
gameObjectNew:get_rigidbody2D():bindTransform(tr)
gameObjectNew:get_rigidbody2D():bindScene(mainSc)
gameObjectNew:get_rigidbody2D().type = rigidbodyType.STATIC

mainSc:addObject(objectRef(gameObjectNew))

print("engine started omg")