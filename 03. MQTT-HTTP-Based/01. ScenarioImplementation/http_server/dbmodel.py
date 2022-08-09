
from flask_sqlalchemy import SQLAlchemy
from flask_marshmallow import Marshmallow


db = SQLAlchemy()
ma = Marshmallow()


class ActivitySchema(ma.Schema):
    class Meta:
        fields = ('activity_id', 'user_id', 'office_id', 'office', 'datatime', 'type')


class UserSchema(ma.Schema):
    class Meta:
        fields = ('id', 'password', 'light', 'office_id', 'room_id')


