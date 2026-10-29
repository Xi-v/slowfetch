### What it looks like
**Example 1:**
<img width="1445" height="583" alt="image" src="https://github.com/user-attachments/assets/c3b4fca9-dee4-41e3-a6d9-a0e60d4f1994" />



### How to customise
You can customise what shows when you run the command using the `config.jsonc` file that is present when you clone the repo.
The colours work by RGB values and the modules work by putting them in layout. 
Layout works from top to bottom so be careful how you order it.

### How often this tool updates
whenever i feel like it.

### How to install
```bash
git clone https://github.com/Xi-v/slowfetch.git
cd slowfetch
./slowfetch
```
And then you can add it to aliases.
**Example:**
```bash
alias slowfetch='cd *directory it's installed in* && ./slowfetch && cd "$OLDPWD" >/dev/null'
```
