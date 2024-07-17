// Example: VS Code Extension for Syntax Highlighting
const vscode = require('vscode');

function activate(context) {
    vscode.languages.registerHoverProvider('myLanguage', {
        provideHover(document, position, token) {
            const range = document.getWordRangeAtPosition(position);
            const word = document.getText(range);
            return new vscode.Hover(`Hovering over ${word}`);
        }
    });

    const provider = {
        provideCompletionItems(document, position) {
            const line = document.lineAt(position);
            const completionItem = new vscode.CompletionItem('sampleCompletion');
            return [completionItem];
        }
    };

    context.subscriptions.push(vscode.languages.registerCompletionItemProvider('myLanguage', provider));
}

exports.activate = activate;
