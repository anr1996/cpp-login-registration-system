
/*
Schema for the C++ login and registration system (PostgreSQL 16).
Usage: psql -U postgres -d login_app -f db/schema.sql
*/

CREATE EXTENSION IF NOT EXISTS citext;

CREATE TABLE IF NOT EXISTS users (
	id		SERIAL PRIMARY KEY,
	username	CITEXT UNIQUE NOT NULL,
	email		VARCHAR(255) UNIQUE NOT NULL,
	password_hash   TEXT NOT NULL,
	created_at	TIMESTAMPTZ NOT NULL DEFAULT now(),
	first_name	TEXT NOT NULL,
	last_name	TEXT NOT NULL,
	date_of_birth	DATE NOT NULL,
	phone_number	TEXT NOT NULL CHECK (phone_number ~ '^[0-9]{10}$'),
	gender		TEXT NOT NULL CHECK (gender IN ('male', 'female', 'non_binary', 'other')),
	location	TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS posts (
	id		SERIAL PRIMARY KEY,
	user_id 	INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
	body		TEXT NOT NULL CHECK (char_length(body) BETWEEN 1 AND 500),
	created_at	TIMESTAMPTZ NOT NULL DEFAULT now()
);

CREATE INDEX IF NOT EXISTS posts_user_created_idx ON posts (user_id, created_at DESC);

/*
City labels for location validation and autocomplete, such as 'Keizer, OR, US'.
Not populated here. Load it from GeoNames (CC by 4.0), see the README.
*/
CREATE TABLE IF NOT EXISTS cities (
	label CITEXT PRIMARY KEY
);


/*
A row means follower_id follows followee_id.
The composite primary key prevents duplicate follows, and the CHECK prevents self-follows
*/
CREATE TABLE IF NOT EXISTS follows (
	follower_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
	followee_id INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,
	created_at TIMESTAMPTZ NOT NULL DEFAULT now(),
	PRIMARY KEY (follower_id, followee_id),
	CHECK (follower_id <> followee_id)
);

CREATE INDEX IF NOT EXISTS follows_followee_idx ON follows (followee_id);


