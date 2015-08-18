========================================================================
  Pokec social network
========================================================================

Pokec is the most popular Slovak on-line social network. These datasets
are anonymized and contains relationships and user profile data of the
whole network. Profile data are in Slovak language. Friendships in the
Pokec network are oriented. Datasets were crawled during MAY 25-27 2012.

Author: Lubos Takac, lubos.takac@gmail.com

////////////////////////////////////////////////////////////////////////
DATASET STATISTICS:

Nodes ............................  1632803
Edges ............................ 30622564
Nodes in largest WCC .............  1632803 (1.000)
Edges in largest WCC ............. 30622564 (1.000)
Nodes in largest SCC .............  1304537 (0.799)
Edges in largest SCC ............. 29183655 (0.953)
Average clustering coefficient ...   0.1094
Number of triangles .............. 32557458
Fraction of closed triangles .....  0.01611
Diameter (longest shortest path) .       11
90-percentile effective diameter .      5.3

////////////////////////////////////////////////////////////////////////
FILES:

soc-pokec-relationships.txt
  Contains friendship relations between users. There is one relation per
  line. Values in the line are tab separated. For example if a row 
  contains 3 5, this means that user 3 has a friend: user 5.
	
soc-pokec-profiles.txt
  Contains profile data in a tab separated form. Columns with no data
  contain the string "null". Data are in Slovak Language. 

  Columns (attributes):
    user_id
    public
    completion_percentage
    gender
    region
    last_login
    registration
    AGE
    body
    I_am_working_in_field
    spoken_languages
    hobbies
    I_most_enjoy_good_food
    pets
    body_type
    my_eyesight
    eye_color
    hair_color
    hair_type
    completed_level_of_education
    favourite_color
    relation_to_smoking
    relation_to_alcohol
    sign_in_zodiac
    on_pokec_i_am_looking_for
    love_is_for_me
    relation_to_casual_sex
    my_partner_should_be
    marital_status
    children
    relation_to_children
    I_like_movies
    I_like_watching_movie
    I_like_music
    I_mostly_like_listening_to_music
    the_idea_of_good_evening
    I_like_specialties_from_kitchen
    fun
    I_am_going_to_concerts
    my_active_sports
    my_passive_sports
    profession
    I_like_books
    life_style
    music
    cars
    politics
    relationships
    art_culture
    hobbies_interests
    science_technologies
    computers_internet
    education
    sport
    movies
    travelling
    health
    companies_brands
    more
	
  Column descriptions:
    user_id:
      integer, users' nicknames were mapped to integers
	  public:
      bool, 1 - all friendships are public  
    completion_percentage:
      integer, percentage proportion of filled values
    gender: 
      bool, 1 - man
    region:
      string, mostly regions in Slovakia (example: "zilinsky kraj,
      kysucke nove mesto" means county Zilina, town Kysucke Nove Mesto,
      Slovakia), some foreign countries (example: "zahranicie, 
      zahranicie - nemecko" means foreign country Germany (nemecko)),
      some Czech regions (example: "ceska republika, cz - ostravsky 
      kraj" means Czech Republic, county Ostrava (ostravsky kraj))
    last_login:
      datetime, last time at which the user has logged in
    registration:
      datetime, time at which the user registered at the site
    age:
      integer, 0 - age attribute not set
  
  Notes:
    Other attributes are free fillable by user. They contain opinions
    in text form or URL of club which the user prefer. There are 163
    records without any data and completion_percentage=0. (I think that
    these are	the profiles of people who have canceled their accounts
    during the data crawling.)
////////////////////////////////////////////////////////////////////////