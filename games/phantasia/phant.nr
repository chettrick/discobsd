.de sh
.br
.ne 5
.PP
\fB\\$1\fR
.PP
..
.TH PHANTASIA 6 "May 20, 1986"
.UC 4
.SH NAME
phantasia \- an interterminal fantasy game
.SH SYNOPSIS
.B phantasia
[
.B \-s
] [
.B \-m
] [
.B \-a
] [
.B \-x
] [
.B \-h
] [
.B \-H
] [
.B \-p
]
.SH DESCRIPTION
.I Phantasia
is a role playing game vaguely similar to dungeons and dragons.
It allows players to roll up characters of various types to fight
monsters and other players.
Progression of characters is based upon gaining experience from fighting
monsters (and other players).
.PP
Most of the game is menu driven and self-explanatory (more or less).
The screen is cursor updated, so be sure to set up the
.B TERM
variable in your environment.
.PP
The options provide for a variety of functions to support the game.
They are:
.PP
.TP .5i
.B \-s
Invokes 
.I phantasia
without header information.
.TP .5i
.B \-m
Get a monster listing.
.TP .5i
.B \-a
Get a listing of all character names on file.
.TP .5i
.B \-x
Examine/change a particular character on file.
.TP .5i
.B \-h
Get a help listing (this message).
.TP .5i
.B \-H
Get header listing only.
.TP .5i
.B \-p
Purge old characters.
.PP
The characters are saved on a common file, in order to make the game
interactive between players.  The characters are given a password
in order to retrieve them later.  Only characters above
.B level
zero are saved.  Characters unused for more than nine days will be
purged.
.SH AUTHOR
Edward Estes, AT&T Teletype Corp.
.SH PARTICULARS
.sh "Playing in General"
Certain of the player's more important statistics are almost always
displyed on the screen, with maximums (where applicable) in
parentheses.
.PP
The character is placed randomly near the center of a cartesian
system.  One may move by hitting
.B E, W, N,
or
.B S
(lower case may also be used, at no time is the game case dependent).
To move to a particular (x,y) coordinate, use the
.B move
('1') command.	The distance a character can move is calculated by
1 plus 1.5 per
.B level.
Moving in a particular compass direction will move the player the maximum
allowed distance in that direction.
.PP
A player may at any time see who else is playing with a
.B players
('2') option.  One may see only those who are the same distance or closer
to the origin as he/she.
.B Kings,
and
.B council of the wise
can see and can be seen by everyone.  A
.B palantir
removes these retrictions.
.PP
One can talk to other players with the
.B talk
('3') option.  In general, this is a line or so of text.  To remove a current
message, just type <return> when prompted for a message.
.PP
The
.B stats
('4') option shows a players characteristics in more detail.
.PP
One may leave the game either with the
.B quit
('5') option, or by hitting interrupt.	Quitting during battle results
in death for obvious reasons.
.PP
One may rest by default.  Resting lets one regain maximum
.B energy level,
and also lets one find
.B manna
(more is found for larger levels and further distances from the origin).
.PP
One may call a monster by hitting '9'.
.PP
Several other options become available as the player progress in
.B level
and
.B magic,
or to other positions in the game (
.B valar, council of the wise, king
).
These are described elsewhere.
In general, a control-L will force the redrawing of the screen.
.PP
Other things which may happen are more or less self-explanatory.
.sh "Fighting Monsters"
A player has several options while fighting monsters.  They are as follows:
.TP 1.5i
.B melee
Inflicts damage on the monster, based upon
.B strength.
Also decreases the monster's
.B strength
some.
.TP 1.5i
.B skirmish
Inflicts a little less damage than
.B melee,
but decreases the monster's
.B quickness
instead.
.TP 1.5i
.B evade
Attempt to run away.  Success is based upon both the player's and
the monster's
.B brains
and
.B quickness.
.TP 1.5i
.B spell
Several options for throwing spells (described elsewhere).
.TP 1.5i
.B nick
Hits the monster one plus the player's
.B sword,
and gives the player 10% of the monster's
.B experience.
Decreases the monster's
.B experience
an amount proportional to the amount granted.
This also increases the monster's quickness.
Paralyzed monsters wake up very fast when nicked.
.TP 1.5i
.B luckout
This is essentially a battle of wits with the monster.	Success is based
upon the player's and the monster's
.B brains.
The player gets credit for slaying the monster if he/she succeeds.
Otherwise, nothing happens, and the chance to
.B luckout
is lost.
.sh "Character Statistics"
.TP 1.5i
.B strength
determines how much damage a character can inflict.
.TP 1.5i
.B quickness
determines how many chances a character gets to make decisions while
fighting.
.TP 1.5i
.B energy level
specifies how much damage a character may endure before dying.
.TP 1.5i
.B magic level
determines which spells a character may throw, and how effective those
spells will be.
.TP 1.5i
.B brains
basically, the character's intelligence; used for various fighting options
and spells.
.TP 1.5i
.B manna
used as a power source for throwing spells.
.TP 1.5i
.B experience
gained by fighting monsters and other characters.
.TP 1.5i
.B level
indicative of how much experience a character has accumulated; progresses
geometrically as
.B experience
increases.
.TP 1.5i
.B poison
sickness which degrades a character's performance (affects
.B energy level
and
.B strength
).
.TP 1.5i
.B sin
accumulated as a character does certain nasty things; used only rarely
in normal play of the game.
.TP 1.5i
.B age
number of seconds of playing time for the character.
As
.B age
increases, many personal statistics degenerate.
.sh "Character Types"
Character statistics are rolled randomly from the above list, according
to character type.  The types are as follows:
.TP 1.5i
.B magic user
strong in
.B magic level
and
.B brains
, weak in other areas.	Must rely on wits and magic to survive.
.TP 1.5i
.B fighter
good in
.B strength
and
.B energy level
, fairly good in other areas.  This adds up to a well-equipped fighter.
.TP 1.5i
.B elf
very high
.B quickness
and above average
.B magic level
are
.B elves
selling points.
.TP 1.5i
.B dwarf
very high
.B strength
and
.B energy level
, but with a tendency to be rather slow and not too bright.
.TP 1.5i
.B halfling
rather quick and smart, with high
.B energy level
, but poor in
.B magic
and
.B strength.
Born with some
.B experience.
.TP 1.5i
.B experimento
very mediocre in all areas.  However, the
.B experimento
may be placed almost anywhere within the playing grid.
.PP
The possible ranges for starting statistics are summarized in
the following table.
.PP
.TS
l c c c c c c
l c c c c c c.
Type	Strength	Quick	Manna	Energy	Brains	Magic
_
Mag. User	20-25	30-35	50-100	30-45	60-85	5-9
Fighter	40-55	30-35	30-50	45-70	25-45	3-6
Elf	35-45	28-38	45-90	30-50	40-65	4-7
Dwarf	50-70	25-30	25-45	60-100	20-40	2-5
Halfling	20-25	34	25-45	55-90	40-75	1-4
Experimento	25	27	100	35	25	2
.TE
.PP
Not only are the starting characteristics different for the different
character types, the characteristics progress at different rates for the
different types as the character goes up in
.B level.  Experimentoes'
characteristics progress randomly as one of the other types.
The progression as characters increase in
.B level
is summarized in the following table.
.PP
.TS
l c c c c c
l n n n n n.
Type	Strength	Manna	Energy	Brains	Magic
_
Mag. User	2.0	75	20	6	2.75
Fighter	3.0	40	30	3.0	1.5
Elf	2.5	65	25	4.0	2.0
Dwarf	5	30	35	2.5	1
Halfling	2.0	30	30	4.5	1
.TE
.PP
Character types are identified by certain numeric values as follows:
.br
1:
.B Magic User
2:
.B Fighter
3:
.B Elf
4:
.B Dwarf
5:
.B Halfling
6:
.B Experimento
.PP
Characters with one or more
.B crowns
are designated as a negative type.
.B Kings
have ten added to their type; members of the
.B council of the wise
have twenty added to their type.
.B Valar
are type 99, and
.B ex-valar
are type 90.
.sh "Spells"
During the course of the game, the player may exercise his/her particular
magic powers.  These cases are described below.
.TP 1.5i
.B cloak
.I magic level necessary:
15 (plus level 5)
.br
.I manna used:
35 plus 3 per rest period
.br
Used during normal play.  Prevents monsters from finding the character,
as well as hiding the player from other players.  His/her coordinates
show up as '?' in the
.B players
option.  Players cannot collect
.B manna,
find trading posts, or discover the
.B grail
while cloaked.	Calling a monster uncloaks, as well as choosing
this option while cloaked.
.br
.TP 1.5i
.B teleport
.I magic level necessary:
25 (plus level 10)
.br
.I manna used:
20 per 75 moved
.br
Used during normal play.  Allows the player too move with much more freedom
than with the
.B move
option, at the price of expending manna.  The maximum distance possible
to move is based upon
.B level
and
.B magic level.
.TP 1.5i
.B power blast
.I magic level necessary:
none
.br
.I manna used:
5 times
.B level
.br
Used during inter-terminal battle.  Damage is based upon
.B magic level
and
.B strength.
Hits much harder than a normal hit.
.TP 1.5i
.B all or nothing
.I magic level necessary:
none
.br
.I manna used:
1
.br
Used while combatting monsters.
Has a 25% chance of working.  If it works it hits the monster just enough
to kill it.  If it fails, it doesn't hit the monster, and doubles the
monster's
.B quickness
and
.B strength.
Paralyzed monsters wake up much quicker as a result of this spell.
.TP 1.5i
.B magic bolt
.I magic level necessary:
3
.br
.I manna used:
variable
.br
Used while combatting monsters.  Hits the monster based upon the amount
of
.B manna
expended and
.B magic level.
Guaranteed to hit at least 10 per
.B manna.
.TP 1.5i
.B force field
.I magic level necessary:
7
.br
.I manna used:
20
.br
Used during monster combat.  Throws up a shield to protect from damage.
The shield is added to actual energy level, and is a fixed number, based
upon maximum energy.  Normally, damage occurs first to the shield, and
then to the players actual
.B energy level.
.TP 1.5i
.B transform
.I magic level necessary:
10
.br
.I manna used:
35
.br
Used during monster combat.  Transforms the monster randomly into one
of the other 100 monsters from the monster file.
.TP 1.5i
.B increase might
.I magic level necessary:
15
.br
.I manna used:
55
.br
Used during combat with monsters.  Increases strength up to a certain maximum.
.TP 1.5i
.B invisibility
.I magic level necessary:
20
.br
.I manna used:
45
.br
Used while fighting monsters.  Makes it harder for the monster to hit,
by temporarily increasing the player's
.B quickness.
This spell may be thrown several times, but a maximum level will be reached.
.TP 1.5i
.B transport
.I magic level necessary:
25
.br
.I manna used:
50
.br
Used during monster combat.  Transports the monster away from the
player.  Success is base upon player's
.B magic
and
.B brains,
and the monster's
.B experience.
If it fails the player is transported instead.	60% of the time, the monster
will drop any treasure it was carrying.
.TP 1.5i
.B paralyze
.I magic level necessary:
30
.br
.I manna used:
60
.br
Used during monster combat.  "Freezes" the monster by putting its
.B quickness
slightly negative.  The monster will slowly wake up.  Success is based
upon player's
.B magic
and the monster's
.B experience.
If it fails, nothing happens.
.TP 1.5i
.B specify
.I magic level necessary:
none
.br
.I manna used:
1000
.br
Used during monster combat only by
.B valar
or
.B council of the wise.
Allows the player to pick which monster to fight.
.sh "Monsters"
Monsters get bigger as one moves farther from the origin (0,0).  Rings of
distance 125 from the origin determine the size.  A monster's
.B experience, energy level,
and
.B brains
are multiplied by the size.
.B Strength
is increase 50% per size over one, and
.B quickness
remains the same, regardless of size.
.PP
Also, meaner monsters are to be found as one progress farther out
from the origin.  Monsters also may flock.  The percent chance of that
happening is designated as
.B flock%
in the monster listing.  Monsters outside the first ring
may carry treasure, as determined by their treasure type.
Flocking monsters, and bigger monsters increase the chances of treasure.
.PP
Certain monsters have special abilities; they are as follows:
.TP 1.5i
.B Unicorn
can only be subdued if the player is in possession of a
.B virgin.
.TP 1.5i
.B Modnar
has random characteristics, including treasure type.
.TP 1.5i
.B Mimic
will pick another name from the list of monsters in order to
confuse.
.TP 1.5i
.B Dark Lord
very nasty person.  Does not like to be hit (especially nicked),
and many spells do not work well against him.
One can always
.B evade
from the
.B Dark Lord.
.TP 1.5i
.B Leanan-Sidhe
also a very nasty person.  She will permanently sap
.B strength
from someone.
.TP 1.5i
.B Saruman
wanders around with
.B Wormtongue
, who can steal a
.B palantir.
Also,
.B Saruman
may turn a player's gems into gold pieces,
or scramble her/his stats.
.TP 1.5i
.B Thaumaturgist
can transport a player.
.TP 1.5i
.B Balrog
inflicts damage by taking away
.B experience
, not
.B energy.
.TP 1.5i
.B Vortex
may take some
.B manna.
.TP 1.5i
.B Nazgul
may try to steal a
.B ring
or neutralize part of one's
.B brains.
.TP 1.5i
.B Tiamat
may take half a players
.B gold
and
.B gems
and escape.
.TP 1.5i
.B Kobold
may get nasty and steal one gold piece and run away.
.TP 1.5i
.B Shelob
may bite, inflicting the equivalent of one
.B poison.
.TP 1.5i
.B Assorted Faeries
These are killed if attacking someone carrying
.B holy water.
These are
.B Cluricaun, Fir Darrig, Fachan,
.B Ghille Dhu, Bogle, Killmoulis,
and
.B Bwca.
.TP 1.5i
.B Lamprey
may bite, inflicting 1/2 of a
.B poison.
.TP 1.5i
.B Shrieker
will call one of its (much bigger) buddies if picked upon.
.TP 1.5i
.B Bonnacon
will become bored with battle, fart, and run off.
.TP 1.5i
.B Smeagol
will try to steal a
.B ring
from a player, if given the chance.
.TP 1.5i
.B Succubus
may inflict damage through a
.B force field.
This subtracts from
.B energy level
instead of any shield the player may have thrown up.
This is a very easy way to die.
.TP 1.5i
.B Cerberus
loves metal and will steal all the metal treasures from
a player if able.
.TP 1.5i
.B Ungoliant
can bite and poison.  This inflicts five
.B poisons
, and also takes one from the player's
.B quickness.
.TP 1.5i
.B Jabberwock
may tire of battle, and leave after calling one of his friends
(
.B Jubjub Bird
or
.B Bandersnatch
).
.TP 1.5i
.B Morgoth
actually
.B Modnar
, but reserved for
.B council of the wise, valar,
and
.B ex-valar.
Fights with
.B Morgoth
end when either he or the player dies.	His characteristics
are calculated based upon the player's.  The player is given
the chance to ally with him.  No magic, except
.B force field
works when battling
.B Morgoth.
.TP 1.5i
.B Troll
may regenerate its
.B energy
and
.B strength
while in battle.
.TP 1.5i
.B Wraith
may make a player blind.
.sh "Treasures"
Various treasure types are as follows:
.TP 1.5i
.B Type zero
.I none
.TP 1.5i
.B Type one
.I power booster
\- adds manna.
.br
.I druid
\- adds experience.
.br
.I holy orb
\- subtracts 0.25 sin.
.TP 1.5i
.B Type two
.I amulet
\- protects from cursed treasure.
.br
.I holy water
\- kills
.B assorted faeries.
.br
.I hermit
\- reduces sin by 25% and adds some manna.
.TP 1.5i
.B Type three
.I shield
\- adds to maximum
.B energy level
.br
.I virgin
\- used to subdue a
.B unicorn
, or to give much
.B experience
(and some
.B sin
).
.br
.I athelas
\- subtracts one
.B poison.
.TP 1.5i
.B Type four (scrolls)
.I shield
* \- throws a bigger than normal
.B force field.
.br
.I invisible
* \- puts the finder's
.B quickness
to one million.
.br
.I ten fold strength
* \- multiplies finder's strength by ten.
.br
.I pick monster
\- allows finder to pick next monster to battle.
.br
.I general knowledge
\- adds to finder's
.B brains
and
.B magic level.
.PP
All the scrolls except
.B general knowledge
automatically call a monster.  Those that are marked with a * preserve any
spells that were already in effect.  Those that call monsters are only in
effect while in battle.
.TP 1.5i
.B Type five
.I dagger
\- adds to
.B strength.
.br
.I armour
\- same as a
.B shield,
but bigger.
.br
.I tablet
\- adds brains.
.TP 1.5i
.B Type six
.I priest
\- rests to maximum; adds 
.B manna, brains;
and halves
.B sin.
.br
.I Robin Hood
\- increases
.B shield
and adds permanently to
.B strength.
.br
.I axe
\- like
.B dagger,
but bigger.
.TP 1.5i
.B Type seven
.I charm
\- protects from cursed treasure (used before
.B amulet
); used in conjunction with
.B blessing
to battle
.B Dark Lord.
.br
.I Merlyn
\- adds
.B brains, magic,
and
.B manna.
.br
.I war hammer
\- like an
.B axe,
but bigger.
.TP 1.5i
.B Type eight
.I healing potion
\- sets
.B poison
to -2, or subtracts two from
.B poison,
whichever is better.
.br
.I transporter
\- allows finder to move anywhere.
.br
.I sword
\- like a
.B war hammer
, but bigger.
.TP 1.5i
.B Type nine
.I golden crown
\- allows the player to become
.B king,
by going to (0,0).
.br
.I blessing
\- cuts
.B sin
to 1/3, adds
.B manna,
rests to max., and kills
.B Dark Lord
with a
.B charm.
.br
.I quicksilver
\- adds to 
.B quickness.
.TP 1.5i
.B Type ten
.I elven boots
\- adds permanently to
.B quickness.
.TP 1.5i
.B Type eleven
.I palantir
\- allows one to see all the other players; used by
.B council of the wise
to seek the
.B grail.
.TP 1.5i
.B Type twelve/thirteen
.I ring
\- allows one to hit much harder in battle, etc.
.PP
Any treasure type 10-13 monsters may instead carry a type nine treasure.
.PP
A monster may also be carrying
.B gold
or
.B gems.
These are used at
.B trading posts
to buy things.	A
.B gem
is worth 1000 gold pieces.  Too much
.B gold
will slow a player down.  One may carry 1000 plus 200 per
.B level
of
.B gold.
A
.B gem
weighs one half a gold piece.
Monsters of treasure type 7 or higher may carry
.B gems.
.PP
The chance of a cursed treasure is based upon treasure type.
The more valuable treasures have a greater chance of being cursed.
A cursed treasure knocks
.B energy level
very low, and adds 0.25
.B poison.
.sh "Rings"
.B Rings
are only carried by
.B nazguls
and
.B Dark Lord.
They come in four different flavors.
All
.B rings
rest the player to maximum and cause him/her to hit much harder
in battle with monsters (assuming one has chosen to use the
.B ring
for battle.)
.PP
Two types of
.B rings
are cursed and come either from
.B nazguls
or
.B Dark Lord.
After a few times of using these types, the player falls
under the control of the
.B ring,
and strange, random things will occur.
Eventually, the player dies, and gives his/her name to a monster
on the file.
Dying before the
.B ring
is used up also renames the monster.
.PP
The two remaining types of
.B rings
are much more benign.
The one from a
.B nazgul
is good for a limited number of battle rounds, and will save
the player from death if it was being used when he/she died.
The one from
.B Dark Lord
is the same, except that it never is used up.
.B rings
disappear after saving someone from death.
In general, cursed
.B rings
occur much more often than normal ones.
It is usually not a good idea to pick one up.
The only way to get rid of a
.B ring
is to have a monster steal it.
.sh "King"
A player may become
.B king by finding a
.I crown
and going to (0,0).  Players must have a
.B level
in the range of 10 to 1000 to be able to find a
.I crown.
.PP
Once a player is king, he/she may do certain things while in 
the Lord's Chamber (0,0).  These fall under the
.B decree
('0') option.
.TP 1.5i
.I transport
This is done to another player.  It randomly moves the affected
player about.  A
.B charm
protects from transports.
.TP 1.5i
.I curse
This is done to another player.  It is analogous to cursed treasure,
but worse.  It inflicts two
.B poison,
knocks
.B energy level
very low, and degrades the maximum energy.  It also
removes a
.B cloak.
A
.B blessing
protects from king's curses.
.TP 1.5i
.I energy void
The king may put as many of these (within reason) scattered about
his/her kingdom as he/she pleases.
If a player hits one, he/she loses
.B manna, energy,
and
.B gold.
The energy void disappears after being hit.
.TP 1.5i
.I bestow
This is also done to another player.  The king may
wish to reward one or more loyal subjects by sharing his/her
riches (
.B gold
).  Or it is a convenient way to dispose of some unwanted
deadweight.
.TP 1.5i
.I collect taxes
Everyone pays 10% tax on all
.B gold
collected, regardless of the existence of a
.B king.
The king may collect this amount with this option.
.PP
The
.B king
may also 
.B teleport
anywhere for free by using the origin as a starting place.
.sh "Special Places"
Certain regions of the playing grid have different names.
In general, this is only to give the player some idea of
his/her present location.  Some special places do exist.
.TP 1.5i
.I Trading Posts
These are located at |x| == |y| == n*n*100 for n = 1, 2...1000.
Trading posts farther out have more things for sale.
Be careful about cheating merchants there, as they have short
tempers.
Merchants are dishonest about 5% of the time.
.TP 1.5i
.I Lord's Chamber
This is located at (0,0).  Only players with
.B crowns
may enter.
.TP 1.5i
.I Point of No Return
This is located beyond 1.2e+6 in any direction.
The only way to return from here is a
.B transporter
or to have a
.B valar
relocate the player.
.TP 1.5i
.I Dead Marshes
This is a band located fairly distant from the origin.	The first
fourteen monsters (water monsters) can normally only be found here.
.TP 1.5i
.I Valhala
This place is where the
.B valar
resides.  It is associated with no particular coordinate on the
playing grid.
.TP 1.5i
.I Wormholes
At fixed locations on the grid are several holes to underground
defects in the playing area.
Sixty-four chambers exist; sixteen of which open to the outside
world.
While in the wormholes, one move by going forward, backward,
left, or right.
One can always undo a move by going in the opposite direction.
The wormholes are mappable.
For example, to move from the first wormhole {at (-400,0)} to
the second wormhole, type 'LL'.
.sh "Miscellaneous"
There are several bits of trivial knowledge which fall under this category.
.PP
A
.I guru
will never be disgusted with your
.B sins
if they are less than one.
.PP
A
.I medic
wants half of a player's
.B gold
to be happy.  Offering more than one has, or a negative amount
will anger the
.I medic,
who will make the player worse (add one
.B poison
).
.PP
The
.B Holy Grail
does little for those who are not ready to behold it.
Whenever anyone finds it, it moves.
It is always located within 1e+6 in any compass direction of the origin.
.PP
There is a maximum amount of
.B manna
and
.B charms
a player may posses, based upon
.B level.
.I Quicksilver
is always limited to to a maximum of 99.
.PP
.I Books
bought at a
.B trading post
increase 
.B brains,
based upon the number bought.
It is unwise, however to buy more than 1/10 of one's
.B level
in books at a time.
.PP
Players over level 10000 are automatically retired.
.PP
A
.I blindness
goes away in random time.
.sh "Inter-terminal Battle"
When two player's coordinates correspond, they may engage in battle.
In general, the player with the highest
.B quickness
gets the first hit.
If the two players are severely mis-matched, the stronger player
is drastically handicapped for the battle.
In order to protect from being stuck in an infinite loop,
the player waiting for response may time out.  Options for battle are:
.TP 1.5i
.I fight
Inflicts damage upon other person.
.TP 1.5i
.I run away
Escape from battle.  Has a 75% chance of working.
.TP 1.5i
.I power blast
Battle spell.
.TP 1.5i
.I luckout
One-time chance to try to win against the foe.	Has a 10% chance of working.
.PP
Sometimes waits for the other player may be excessive, because
he/she may be battling a monster.  Upon slaying a player in battle
the winner gets the other's
.B experience
and treasures.
.B Rings
do not work for inter-terminal battle.
.sh "Council of the Wise, Valar"
A player automatically becomes a member of the
.B council of the wise
upon reaching level 3000.  Members of the council cannot have
.B rings.
Members of the council have a few extra options which they can exercise.
These fall under the
.B intervention
('8') option.
One is to
.I heal
another player.  This is just a quick way for that player to be rested
to maximum and lose a little
.B poison.
The main purpose in life for members of the council is to seek the
.B Holy Grail.
This is done with a
.B palantir
under the
.I seek grail
option.  The distance cited by the seek is accurate within 10%, in order
not to make it too easy to find the grail.  Seeking costs 1000
.B manna.
A player must have infintesimally small
.B sin,
or else it's all over upon finding the grail.
In order to help members of the council on their quest, they
may
.I teleport
with greater ease.
.PP
Upon finding the grail, the player advance to position of
.B valar.
He/she may then exercise more and niftier options under
.I intervention.
These include all of the council members' option plus the
ability to move other players about, bless them, and throw monsters at
them.
.B Valar
are essentially immortal, but are actually given five lives.
If these are used up, the player is left to die, and becomes an
.B ex-valar.
.B Valar
cannot
.I move, teleport,
or call monsters.
Any monsters which a
.B valar
encounters are based upon his/her size.
Only one valar may exists at a time.
A player replaces the exiting valar upon finding the grail.
The valar is then bumped back to the council of the wise.
.SH BUGS
Many.  The whole program is a hack.  The handling of incorrectly
exitted characters is a kludge.  The screen is set up assuming a 24 by 80
character screen; no attempt was made to provide otherwise.
If the program is not set uid, it crashes on the first attempt to open a
data file.
If any of the data items get too big for the allotted space on the screen,
no guarantees are made about what will happen.
There should be a way to change a character's name.
