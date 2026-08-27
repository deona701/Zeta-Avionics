import QtQuick
import QtQuick3D

Node {
    id: node

    // Resources
    property url textureData: "maps/textureData.jpg"
    property url textureData8: "maps/textureData8.png"
    property url textureData10: "maps/textureData10.jpg"
    Texture {
        id: _0_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData
    }
    Texture {
        id: _1_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData8
    }
    Texture {
        id: _2_texture
        generateMipmaps: true
        mipFilter: Texture.Linear
        source: node.textureData10
    }
    PrincipledMaterial {
        id: tripo_node_b04adb05_1221_4852_99f7_880985b71b78_material_002_material
        objectName: "tripo_node_b04adb05-1221-4852-99f7-880985b71b78_material.002"
        baseColorMap: _0_texture
        metalnessMap: _1_texture
        roughnessMap: _1_texture
        metalness: 1
        roughness: 1
        normalMap: _2_texture
        cullMode: PrincipledMaterial.NoCulling
        alphaMode: PrincipledMaterial.Opaque
    }

    // Nodes:
    Node {
        id: root
        objectName: "root"
        Model {
            id: tripo_node_b04adb05_1221_4852_99f7_880985b71b78
            objectName: "tripo_node_b04adb05-1221-4852-99f7-880985b71b78"
            source: "meshes/meshes_0__mesh.mesh"
            materials: [
                tripo_node_b04adb05_1221_4852_99f7_880985b71b78_material_002_material
            ]
        }
    }

    // Animations:
}
