import bpy, math, os
from mathutils import Vector

bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete(use_global=False)
for d in list(bpy.data.materials):
    bpy.data.materials.remove(d)

def mat(name, color, metallic=0.0, rough=0.5, emission=None):
    m=bpy.data.materials.new(name)
    m.diffuse_color=(*color,1)
    m.use_nodes=True
    bsdf=m.node_tree.nodes.get('Principled BSDF')
    bsdf.inputs['Base Color'].default_value=(*color,1)
    bsdf.inputs['Metallic'].default_value=metallic
    bsdf.inputs['Roughness'].default_value=rough
    if emission:
        bsdf.inputs['Emission Color'].default_value=(*emission,1)
        bsdf.inputs['Emission Strength'].default_value=2.3
    return m

M_CONC=mat('OB_Concrete',(0.10,0.12,0.14),0.05,0.8)
M_METAL=mat('OB_Metal',(0.05,0.06,0.07),0.75,0.28)
M_COVER=mat('OB_Cover',(0.20,0.23,0.25),0.35,0.55)
M_F1=mat('OB_Floor1',(0.10,0.42,0.75),0.15,0.4,(0.05,0.25,0.8))
M_F2=mat('OB_Floor2',(0.95,0.48,0.08),0.15,0.4,(1.0,0.22,0.02))
M_F3=mat('OB_Security',(0.65,0.08,0.10),0.2,0.38,(0.8,0.02,0.02))
M_EX=mat('OB_Exchange',(0.08,0.70,0.38),0.3,0.35,(0.0,0.9,0.3))
M_GLASS=mat('OB_Glass',(0.05,0.12,0.16),0.1,0.18)

def cube(name, loc, size, material, bevel=0.0, rot=(0,0,0)):
    bpy.ops.mesh.primitive_cube_add(location=loc, rotation=rot)
    o=bpy.context.object
    o.name=name
    o.scale=(size[0]/2,size[1]/2,size[2]/2)
    bpy.ops.object.transform_apply(location=False, rotation=False, scale=True)
    if material: o.data.materials.append(material)
    if bevel>0:
        mod=o.modifiers.new('Bevel','BEVEL'); mod.width=bevel; mod.segments=3
    return o

def cyl(name, loc, radius, depth, material):
    bpy.ops.mesh.primitive_cylinder_add(vertices=20, radius=radius, depth=depth, location=loc)
    o=bpy.context.object; o.name=name; o.data.materials.append(material); return o

W,D,H=24.0,20.0,4.5
for f in range(3):
    z=f*H; fm=[M_F1,M_F2,M_F3][f]
    cube(f'F{f+1}_Slab_Main',(-3,0,z-0.18),(18,D,0.36),M_CONC,0.06)
    cube(f'F{f+1}_Slab_Back',(9,-7.25,z-0.18),(6,5.5,0.36),M_CONC,0.06)
    cube(f'F{f+1}_Slab_Front',(9,7.25,z-0.18),(6,5.5,0.36),M_CONC,0.06)
    for name,loc,size in [
        ('Rail_N',(0,-9.85,z+0.55),(24,0.22,1.1)),('Rail_S',(0,9.85,z+0.55),(24,0.22,1.1)),
        ('Rail_W',(-11.85,0,z+0.55),(0.22,20,1.1)),('Rail_E',(11.85,0,z+0.55),(0.22,20,1.1))]:
        cube(f'F{f+1}_{name}',loc,size,M_METAL,0.04)
    for x,y in [(-10,-8),(-10,8),(10,-8),(10,8),(-2,-8),(-2,8)]:
        cube(f'F{f+1}_Column_{x}_{y}',(x,y,z+2.25),(0.55,0.55,4.5),M_METAL,0.07)
    cube(f'F{f+1}_IdentityStrip',(-10.9,0,z+1.7),(0.16,7.5,0.20),fm,0.03)
    covers=[(-7,-5,2.4,0.7,1.25),(-6.5,4.5,1.5,2.2,1.25),(-2,-4,3.0,0.65,1.05),(1.5,5,0.65,3.0,1.05),(4,-6,1.8,1.0,1.3),(4.5,6,2.2,0.8,1.3)]
    for i,(x,y,sx,sy,sz) in enumerate(covers):
        cube(f'F{f+1}_Cover_{i}',(x,y,z+sz/2),(sx,sy,sz),M_COVER,0.12)
    cube(f'F{f+1}_ExchangeBase',(-9.5,0,z+0.45),(1.35,1.35,0.9),M_EX,0.15)
    cube(f'F{f+1}_ExchangeScreen',(-8.78,0,z+1.15),(0.12,0.85,0.65),M_GLASS,0.06)
    for i,(x,y) in enumerate([(-7.5,-5.5),(-7.5,5.5),(-2.5,-6.5),(-2.5,6.5)]):
        cyl(f'F{f+1}_Spawn_{i+1}',(x,y,z+0.035),0.42,0.07,fm)

for f in range(2):
    z=f*H; run=9.0; rise=H; length=(run*run+rise*rise)**0.5; angle=math.atan2(rise,run)
    cube(f'Ramp_F{f+1}_to_F{f+2}',(6,0,z+rise/2),(3.9,length,0.30),M_METAL,0.07,rot=(angle,0,0))
    for x in (4.0,8.0):
        cube(f'RampRail_{f}_{x}',(x,0,z+rise/2+0.55),(0.16,length,1.0),M_COVER,0.04,rot=(angle,0,0))

cube('Core_BackWall',(9.8,0,4.5),(0.35,8.0,9.0),M_METAL,0.08)
for z in (1.0,5.5,10.0): cube(f'Core_Light_{z}',(9.6,0,z),(0.18,4.8,0.18),M_EX,0.03)
for z in (4.1,8.6,13.1): cube(f'Beam_{z}',(0,0,z),(24,0.35,0.35),M_METAL,0.05)

# world / lighting
bpy.context.scene.world.color=(0.015,0.02,0.03)
bpy.ops.object.light_add(type='AREA', location=(0,0,15))
key=bpy.context.object; key.name='OB_KeyLight'; key.data.energy=2200; key.data.shape='DISK'; key.data.size=14
bpy.ops.object.light_add(type='AREA', location=(-8,-6,7))
fill=bpy.context.object; fill.name='OB_FillLight'; fill.data.energy=1000; fill.data.size=10

# preview camera
bpy.ops.object.camera_add(location=(30,-34,23))
cam=bpy.context.object; cam.name='OB_PreviewCamera'
cam.rotation_euler=(Vector((0,0,4.5))-cam.location).to_track_quat('-Z','Y').to_euler()
bpy.context.scene.camera=cam
bpy.context.scene.render.engine='BLENDER_EEVEE_NEXT'
bpy.context.scene.render.resolution_x=1280; bpy.context.scene.render.resolution_y=720; bpy.context.scene.render.resolution_percentage=100

base=os.path.abspath(os.path.join(os.path.dirname(__file__),'..','..'))
blend_path=os.path.join(base,'Generated','Blender','OpenBattle_PrototypeTower.blend')
fbx_path=os.path.join(base,'Content','OpenBattle','Art','PrototypeTower','SM_OpenBattle_PrototypeTower.fbx')
render_path=os.path.join(base,'Generated','Blender','OpenBattle_PrototypeTower_preview.png')

bpy.ops.wm.save_as_mainfile(filepath=blend_path)
# export meshes only
bpy.ops.object.select_all(action='DESELECT')
for o in bpy.context.scene.objects:
    if o.type=='MESH': o.select_set(True)
bpy.ops.export_scene.fbx(filepath=fbx_path,use_selection=True,apply_unit_scale=True,apply_scale_options='FBX_SCALE_ALL',axis_forward='-Y',axis_up='Z')
bpy.context.scene.render.filepath=render_path
bpy.ops.render.render(write_still=True)
print('BUILT', blend_path)
print('EXPORTED', fbx_path)
print('PREVIEW', render_path)
print('MESH_OBJECTS', sum(1 for o in bpy.context.scene.objects if o.type=='MESH'))
