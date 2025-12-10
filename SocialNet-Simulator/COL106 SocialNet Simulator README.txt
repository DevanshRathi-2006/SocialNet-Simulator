COL106 SocialNet Simulator README
Compilation Instructions
The code file is named socialnet.cpp in vs code

Use the provided shell script (build.sh) to compile:

text
./build.sh
This will generate an executable named socialnet. If compilation fails, check for error messages.

Running the Program
Start the simulator by executing:

text
./socialnet
The program reads commands from standard input (stdin). Enter commands as specified below, one per line.

Command Reference & Usage
ADD_USER username
Description: Adds a new user (case-insensitive).

Edge Cases:

Duplicate (case-insensitive) usernames are prevented (e.g., "Lakshay" and "lakshay" are identical).

No output is produced if the user already exists.​

ADD_FRIEND username1 username2
Description: Establishes bidirectional friendship.

Edge Cases:

Both usernames must already exist, case-insensitive.

No output is produced if either user does not exist.

If they are already friends, nothing is updated/output.

Self-friendship is ignored (user cannot friend themselves).​

LIST_FRIENDS username
Description: Prints friends of the user, sorted alphabetically (by original input).

Edge Cases:

If user does not exist, no output.

If user has no friends, outputs nothing.

Output is strictly alphabetically sorted.​

SUGGEST_FRIENDS username N
Description: Suggests up to N friends-of-friends (FoFs), ranked by # mutual friends (descending). Ties sorted alphabetically.

Edge Cases:

If N is 0, output nothing.

If user does not exist, output nothing.

If fewer than N candidates exist, output all available.

Self-suggestion and already friends are excluded.

Output is space-separated, not comma-separated, and ends with a newline.​

DEGREES_OF_SEPARATION username1 username2
Description: Computes shortest path length in friend network.

Edge Cases:

If either user does not exist, print -1.

If both usernames are identical, print 0.

If no path exists between the users, print -1.​

ADD_POST username "post content"
I have assumed that the posts will be entered in input in inverted commas only and handled such cases accordingly in the code
Description: Adds a post to the user's timeline.

Edge Cases:

Only valid if post content is enclosed in quotes ("...").

If username does not exist, or format is invalid, do nothing/output nothing.

Post content and usernames are case-insensitive.

Leading/trailing spaces or quotes handled according to PDF specs.​

OUTPUT_POSTS username N
No output if input is not in inverted commas ..   (also here inputs are strings only).. 
Description: Prints most recent N posts in reverse chronological order.

Edge Cases:    These are the edge cases i have used in the assignment

If username does not exist, no output.

If N is -1, print all posts, most recent first.

If N exceeds the number of posts, print all available posts.

Posts are presented in correct time order, as required.

If no posts, outputs nothing.​

General Notes & Behaviors
All commands and usernames are case-insensitive.

Hash maps are used only for mapping usernames; all other data structures (AVL Tree, Graph) are implemented in your code.

Invalid or incorrectly formatted commands are ignored silently.

All input/output is via stdin/stdout; no files used for input/output.