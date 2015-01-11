import bb.cascades 1.2

Container {
    attachedObjects: TextStyleDefinition {
        id: customFontFace
        rules: FontFaceRule {
            fontFamily: "MyCustomFont"
            source: "asset:///fonts/MyriadPro.otf"
        }
    }
}
