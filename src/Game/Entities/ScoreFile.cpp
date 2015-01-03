#include <Game/Entities/ScoreFile.hpp>
#include <Engine/Helpers/Utils.hpp>
#include <Engine/Helpers/String.hpp>
#include <Engine/Helpers/File.hpp>
#include <Engine/Helpers/Base64.hpp>
#include <Engine/Helpers/INI.hpp>
#include <Game/Entities/Profile.hpp>

#include <stdlib.h>	  // getenv()
#include <fstream>    // ofstream

std::string ScoreFile::extension = "yetrisscores";

ScoreEntry::ScoreEntry():
	points(0),
	lines(0),
	level(1),
	starting_level(1),
	initial_noise(0),
	invisible(false),
	slide_left(false),
	slide_right(false),
	random_algorithm("regular")
{ }

bool ScoreEntry::isLike(ScoreEntry& other)
{
	return (this->starting_level   == other.starting_level &&
	        this->initial_noise    == other.initial_noise  &&
	        this->invisible        == other.invisible      &&
	        this->slide_left       == other.slide_left     &&
	        this->slide_right      == other.slide_right    &&
	        this->random_algorithm == other.random_algorithm);
}




void ScoreFile::eraseAll()
{
	Utils::File::rm_f(Globals::Profiles::current->scores->file);
}





ScoreFile::ScoreFile():
	highScore(NULL)
{ }

void ScoreFile::load()
{
	// Don't want to preserve these across gameplays!
	this->score.points = 0;

	// Reset high score and current score entries,
	// since we're loading it all from the file.
	this->highScore = NULL;
	this->entries.clear();

	// Score files are dependent of the level name.
	std::string score_file = (Globals::Profiles::current->directory +
	                          "hscores" +
	                          "." +
	                          ScoreFile::extension);

	if (! Utils::File::exists(score_file))
	{
		// Not setting default high score!
		throw ScoreFileException("File '" + score_file + "' doesn't exist");
	}

	// Reading whole file's contents into a buffer
	std::ifstream file;
	file.open(score_file.c_str());

	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	std::stringstream contents;
	contents << Utils::Base64::decode(buffer.str());

	// Parsing file's contents as INI
	INI::Parser ini(contents);

	// If it's a score file from a different major version,
	// how should we react?
	// No need to worry about minor versions.
	std::string version = ini["version"];

	if (version[0] != Globals::version[MAJOR])
	{
		// Not setting default high score!
		throw ScoreFileException("File '" + score_file + "' has an old version format");
	}

	// Going through each group on the INI file
	// (each score the user had)
	for (INI::Level::Sections::const_iterator it = ini.top().ordered_sections.begin();
	     it != ini.top().ordered_sections.end();
	     ++it)
	{
		// This is SOO ugly!
		// We should NOT have to worry about INI parser's internals!
		INI::Level ini_score = (*it)->second;

		ScoreEntry entry;

		entry.points = Utils::String::to<int>(ini_score["points"]);
		entry.lines  = Utils::String::to<int>(ini_score["lines"]);

		entry.level          = Utils::String::to<int>(ini_score["level"]);
		entry.starting_level = Utils::String::to<int>(ini_score["starting_level"]);

		entry.initial_noise = Utils::String::to<int>(ini_score["initial_noise"]);

		entry.invisible   = Utils::String::to<bool>(ini_score["invisible"]);
		entry.slide_left  = Utils::String::to<bool>(ini_score["slide_left"]);
		entry.slide_right = Utils::String::to<bool>(ini_score["slide_right"]);

		entry.random_algorithm = Utils::String::to<std::string>(ini_score["random_algorithm"]);

		this->entries.push_back(entry);
	}

	for (size_t i = 0; i < (this->entries.size()); i++)
	{
		if (this->score.isLike(this->entries[i]))
		{
			this->highScore = &(this->entries[i]);
			break;
		}
	}

	// First high score!
	if (this->highScore == NULL)
	{
		// Not setting default high score!
		// So... what should we do...?
	}
}
void ScoreFile::save()
{
	// Score files are dependent of the level name.
	std::string score_file = (Globals::Profiles::current->directory +
	                          "hscores" +
	                          "." +
	                          ScoreFile::extension);

	// Tries to create file if it doesn't exist.
	// If we can't create it at all let's just give up.
	if (! Utils::File::exists(score_file))
	{
		Utils::File::create(score_file);

		if (! Utils::File::exists(score_file))
			throw ScoreFileException("Could not create file '" + score_file + "'");
	}

	// We'll recreate the whole score file from scratch
	INI::Parser ini;
	ini.create();
	ini.top().addKey("version", std::string(VERSION));

	// Adding each score entry on the file
	for (size_t i = 0; i < (this->entries.size()); i++)
	{
		std::string score_name = "score" + Utils::String::toString(i);
		ini.top().addGroup(score_name);

		ini(score_name).addKey("points",           Utils::String::toString(this->entries[i].points));
		ini(score_name).addKey("lines",            Utils::String::toString(this->entries[i].lines));
		ini(score_name).addKey("level",            Utils::String::toString(this->entries[i].level));
		ini(score_name).addKey("starting_level",   Utils::String::toString(this->entries[i].starting_level));
		ini(score_name).addKey("initial_noise",    Utils::String::toString(this->entries[i].initial_noise));
		ini(score_name).addKey("invisible",        Utils::String::toString(this->entries[i].invisible));
		ini(score_name).addKey("slide_left",       Utils::String::toString(this->entries[i].slide_left));
		ini(score_name).addKey("slide_right",      Utils::String::toString(this->entries[i].slide_right));

		ini(score_name).addKey("random_algorithm", this->entries[i].random_algorithm);
	}

	std::stringstream contents;
	ini.dump(contents);

	std::ofstream file;
	file.open(score_file.c_str());
	file << Utils::Base64::encode(contents.str());
	file.close();
}
bool ScoreFile::handle(ScoreEntry* score)
{
	// No high score until now, we've made it!
	if (! this->highScore)
	{
		this->entries.push_back(*score);
		this->highScore = &(this->entries[this->entries.size() - 1]);
		return true;
	}

	// Wrong game settings?
	if (! score->isLike(*this->highScore))
		return false;

	if ((score->points) > (this->highScore->points))
	{
		this->highScore->points = score->points;
		return true;
	}
	return false;
}

