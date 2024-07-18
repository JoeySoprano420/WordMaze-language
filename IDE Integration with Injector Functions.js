// Example: Bleeding-Edge VS Code Extension for WordMaze Language
const vscode = require('vscode');

function activate(context) {
    // Register a hover provider for 'WordMaze'
    const hoverProvider = vscode.languages.registerHoverProvider('wordmaze', {
        provideHover(document, position, token) {
            const range = document.getWordRangeAtPosition(position);
            const word = document.getText(range);

            // Add advanced hover information with markdown support
            const markdownString = new vscode.MarkdownString();
            markdownString.appendMarkdown(`**Hovering over:** \`${word}\`\n\n`);
            markdownString.appendMarkdown(`*Language:* \`WordMaze\`\n`);
            markdownString.appendMarkdown(`*Position:* \`${position.line}:${position.character}\`\n\n`);
            markdownString.appendMarkdown(`[Click here for more info](https://example.com)`);

            return new vscode.Hover(markdownString);
        }
    });

    // Register a completion item provider for 'WordMaze'
    const completionItemProvider = vscode.languages.registerCompletionItemProvider('wordmaze', {
        provideCompletionItems(document, position, token, context) {
            const line = document.lineAt(position);
            const prefix = line.text.substr(0, position.character).split(/\s+/).pop();

            // Sample completion item
            const completionItem = new vscode.CompletionItem('sampleCompletion', vscode.CompletionItemKind.Text);
            completionItem.documentation = new vscode.MarkdownString('This is a sample completion item for `WordMaze`.');

            // Add more completion items
            const completionItems = [
                new vscode.CompletionItem('anotherCompletion', vscode.CompletionItemKind.Keyword),
                new vscode.CompletionItem('yetAnotherCompletion', vscode.CompletionItemKind.Method)
            ];

            return [completionItem, ...completionItems];
        },
        resolveCompletionItem(item, token) {
            // Provide additional details for a completion item
            item.detail = `Details for ${item.label}`;
            item.documentation = new vscode.MarkdownString(`Documentation for ${item.label}`);
            return item;
        }
    }, '.', ':'); // Trigger completion on '.' and ':'

    // Register a code action provider for 'WordMaze'
    const codeActionProvider = vscode.languages.registerCodeActionsProvider('wordmaze', {
        provideCodeActions(document, range, context, token) {
            const codeAction = new vscode.CodeAction('Sample Code Action', vscode.CodeActionKind.QuickFix);
            codeAction.command = { command: 'wordmazeExtension.sampleCommand', title: 'Run Sample Command' };
            return [codeAction];
        }
    });

    // Register a command
    const sampleCommand = vscode.commands.registerCommand('wordmazeExtension.sampleCommand', () => {
        vscode.window.showInformationMessage('Sample Command Executed');
    });

    // Register a syntax highlighter for 'WordMaze'
    const tokenTypes = new vscode.SemanticTokensLegend(['keyword', 'variable', 'function']);
    const semanticTokensProvider = vscode.languages.registerDocumentSemanticTokensProvider('wordmaze', {
        provideDocumentSemanticTokens(document, token) {
            const builder = new vscode.SemanticTokensBuilder(tokenTypes);
            const text = document.getText();
            const keywords = ['if', 'else', 'for', 'while']; // Example keywords for WordMaze

            text.split(/\s+/).forEach((word, index) => {
                if (keywords.includes(word)) {
                    builder.push(new vscode.Range(new vscode.Position(0, index), new vscode.Position(0, index + word.length)), 'keyword');
                }
            });

            return builder.build();
        }
    }, tokenTypes);

    // Register a document symbol provider for 'WordMaze'
    const documentSymbolProvider = vscode.languages.registerDocumentSymbolProvider('wordmaze', {
        provideDocumentSymbols(document, token) {
            const symbols = [];

            // Example: Extract symbols from the document
            const regex = /(?:function|method)\s+(\w+)\s*\(/g;
            let match;
            while ((match = regex.exec(document.getText()))) {
                const symbolName = match[1];
                const symbolRange = document.getWordRangeAtPosition(document.positionAt(match.index));
                const symbolKind = vscode.SymbolKind.Function; // Adjust symbol kind as needed
                symbols.push(new vscode.DocumentSymbol(symbolName, '', symbolKind, symbolRange, symbolRange));
            }

            return symbols;
        }
    });

    // Register a code lens provider for 'WordMaze'
    const codeLensProvider = vscode.languages.registerCodeLensProvider('wordmaze', {
        provideCodeLenses(document, token) {
            const codeLenses = [];

            // Example: Add code lenses for each function/method
            const regex = /(?:function|method)\s+(\w+)\s*\(/g;
            let match;
            while ((match = regex.exec(document.getText()))) {
                const functionName = match[1];
                const startPosition = document.positionAt(match.index);
                const endPosition = document.positionAt(match.index + match[0].length);
                const range = new vscode.Range(startPosition, endPosition);
                codeLenses.push(new vscode.CodeLens(range));
            }

            return codeLenses;
        },
        resolveCodeLens(codeLens, token) {
            // Example: Resolve additional information for a code lens
            codeLens.command = {
                title: 'Run Test',
                command: 'wordmazeExtension.runTest',
                arguments: [codeLens.range.start]
            };
            return codeLens;
        }
    });

    // Register a code formatter for 'WordMaze'
    const documentFormattingEditProvider = vscode.languages.registerDocumentFormattingEditProvider('wordmaze', {
        provideDocumentFormattingEdits(document, options, token) {
            const textEdits = [];

            // Example: Format the entire document
            const firstLine = document.lineAt(0);
            const lastLine = document.lineAt(document.lineCount - 1);
            const range = new vscode.Range(firstLine.range.start, lastLine.range.end);
            const formattedText = document.getText(range).toUpperCase(); // Example: Convert to uppercase
            textEdits.push(vscode.TextEdit.replace(range, formattedText));

            return textEdits;
        }
    });

    // Push all providers and commands to the context's subscriptions
    context.subscriptions.push(hoverProvider);
    context.subscriptions.push(completionItemProvider);
    context.subscriptions.push(codeActionProvider);
    context.subscriptions.push(sampleCommand);
    context.subscriptions.push(semanticTokensProvider);
    context.subscriptions.push(documentSymbolProvider);
    context.subscriptions.push(codeLensProvider);
    context.subscriptions.push(documentFormattingEditProvider);
}

exports.activate = activate;
